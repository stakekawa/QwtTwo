/*!
 *  \file QwtTwoPlotMarker.h
 *
 *  \brief This file contains the implementation of QwtTwoPlotMarker class
 *  \author Stefano Takekawa s.takekawa_at_gmail.com
 *  \date 2017
 *  \copyright Apache Licence 2.0
 */


#include <QPainter>
#include <qwt_scale_map.h>
#include <qwt_symbol.h>
#include <qwt_text.h>
#include <qwt_math.h>
#include "QwtTwoPlotMarker.h"


/*                                                                            */
/* Private data                                                               */
/*                                                                            */


static constexpr double k_labelRotationDefault = 0.;


/* virtual */
QwtTwoPlotMarker::QwtTwoPlotMarker(const QString& title) :
    QwtPlotMarker(title),
    m_labelRotation(k_labelRotationDefault),
    m_labelRotationUser(k_labelRotationDefault)
{

}


/* virtual */
QwtTwoPlotMarker::QwtTwoPlotMarker(const QwtText& title) :
    QwtPlotMarker( title ),
    m_labelRotation(k_labelRotationDefault),
    m_labelRotationUser(k_labelRotationDefault)
{

}


QwtTwoPlotMarker::~QwtTwoPlotMarker()
{

}



double QwtTwoPlotMarker::labelRotation() const
{
    return m_labelRotationUser;
}


void QwtTwoPlotMarker::setLabelRotation(const double p_angle)
{
    m_labelRotationUser = p_angle;

    if (0 == (int(p_angle) % 360))
    {
        // multiple of 360 degrees reduced to 0 degrees
        m_labelRotation = 0.;
    }
    else if (0 <= p_angle)
    {
        // reduce in the range [0 - 360)
        m_labelRotation = p_angle - double(int(p_angle)/360*360);
    }
    else
    {
        // reduce in the range (-360 - 0) and add 360
        m_labelRotation = 360 + p_angle - double(int(p_angle)/360*360);
    }
}


/* virtual */
void QwtTwoPlotMarker::draw(QPainter* painter,
                            const QwtScaleMap& xMap,
                            const QwtScaleMap& yMap,
                            const QRectF& canvasRect) const
{
    const QPointF pos( xMap.transform( xValue() ),
                       yMap.transform( yValue() ) );

    // draw lines

    QwtPlotMarker::drawLines( painter, canvasRect, pos );

    // draw symbol
    if ( symbol() &&
         ( symbol()->style() != QwtSymbol::NoSymbol ) )
    {
        const QSizeF sz = symbol()->size();

        const QRectF clipRect = canvasRect.adjusted(
                                    -sz.width(), -sz.height(), sz.width(), sz.height() );

        if ( clipRect.contains( pos ) )
            symbol()->drawSymbol( painter, pos );
    }

    // x and y of rect triangol in cartesian world
    const double xScaleUnit( 1. );
    const double yScaleUnit( (90. == m_labelRotation) ? 0. : tan( m_labelRotation * M_PI / 180. ) );

    // compute x and y of rect triangol due to xMap and yMap
    // fabs is used to take into account inverted axis
    const double xMapUnit( fabs( xMap.transform( xScaleUnit ) - xMap.transform( 0. ) ) );
    const double yMapUnit( fabs( yMap.transform( yScaleUnit ) - yMap.transform( 0. ) ) );

    // compute angle correction due to xMap and yMap
    double correctionAngle = ( 0. == yMapUnit ) ? 0. : m_labelRotation - ( atan( yMapUnit / xMapUnit ) * 180. / M_PI );

    // difference due to label written Qt::AlignRight if the rotation
    // angle is greater than 90 degrees
    if (90. < fabs( m_labelRotation ) )
    {
        correctionAngle = ( 0. == yMapUnit ) ? 0. : m_labelRotation + ( atan( yMapUnit / xMapUnit ) * 180. / M_PI ) - 180.;
    }

    // the correction changes in direction if the angle is bigger than 180 degrees
    if (180. < fabs( m_labelRotation ) )
    {
        correctionAngle = ( 0. == yMapUnit ) ? 0. : m_labelRotation - ( atan( yMapUnit / xMapUnit ) * 180. / M_PI ) - 180.;
    }

    // the correction changes in direction if the angle is bigger than 270 degrees
    if (270. < fabs( m_labelRotation ) )
    {
        correctionAngle = ( 0. == yMapUnit ) ? 0. : m_labelRotation + ( atan( yMapUnit / xMapUnit ) * 180. / M_PI ) + 180;
    }

    drawLabel( painter, canvasRect, pos, correctionAngle );
}


/*                                                                            */
/* Protected methos                                                           */
/*                                                                            */


/* virtual */
void QwtTwoPlotMarker::drawLabel(QPainter* painter,
                                 const QRectF&,
                                 const QPointF& pos,
                                 const double& correctionAngle) const
{
    if ( label().isEmpty() )
        return;

    // the actual label rotation angle is corrected to appear
    // aligned with a rect with the same slope
    double angle = m_labelRotation - correctionAngle;

    QPointF alignPos = pos;

    QSizeF symbolOff( 0, 0 );

    // the label is drawn as if it was lineStyle = NoLine

    if ( symbol() &&
         ( symbol()->style() != QwtSymbol::NoSymbol ) )
    {
        symbolOff = symbol()->size() + QSizeF( 1, 1 );
        symbolOff /= 2;
    }

    qreal pw2 = linePen().widthF() / 2.0;
    if ( pw2 == 0.0 )
    {
        pw2 = 0.5;
    }

    const qreal xOff = qMax( pw2, symbolOff.width()  );
    const qreal yOff = qMax( pw2, symbolOff.height() );

    // add the xOff and yOff to the spacing to simplify computations
    const int spacing = QwtPlotMarker::spacing() + std::sqrt( xOff * xOff + yOff * yOff );

    const QSizeF textSize = label().textSize( painter->font() );

    // align is Qt::AlignLeft if the |angle| > 90 degrees or it is
    // Qt::AlignRight if the |angle| <= 90 degrees so the user gets
    // a better view of the label
    Qt::Alignment align = ( 90. >= fabs( angle ) ) ? Qt::AlignLeft : Qt::AlignRight;

    // get original alignment to plot above or below the point
    // Qt::AlignTop is the default

    if ( 270. < m_labelRotation )
    {
        // alignment is inverted in the forth quadrant
        align |= ( labelAlignment() & Qt::AlignBottom ) ? Qt::AlignTop : Qt::AlignBottom;
    }
    else
    {
        align |= ( labelAlignment() & Qt::AlignBottom ) ? Qt::AlignBottom : Qt::AlignTop;
    }

    const float l_halfTextHeight( textSize.height() / 2 );

    if ( align & Qt::AlignLeft )
    {
        angle *= -1.;

        const double l_sin( sin( angle * M_PI / 180. ) );
        const double l_cos( cos( angle * M_PI / 180. ) );

        if ( align & Qt::AlignTop )
        {
            alignPos.rx() += l_sin * l_halfTextHeight + l_cos * spacing;
            alignPos.ry() -= l_cos * l_halfTextHeight - l_sin * spacing;
        }
        else // if ( align & Qt::AlignBottom )
        {
            alignPos.rx() -= l_cos * textSize.width();
            alignPos.ry() -= l_sin * textSize.width();

            alignPos.rx() += l_sin * l_halfTextHeight - l_cos * spacing;
            alignPos.ry() -= l_cos * l_halfTextHeight + l_sin * spacing;
        }
    }
    else // if ( align & Qt::AlignRight )
    {
        angle -= 180.;
        angle *= -1.;

        const double l_sin( sin( angle * M_PI / 180. ) );
        const double l_cos( cos( angle * M_PI / 180. ) );

        if ( align & Qt::AlignTop )
        {
            alignPos.rx() -= l_cos * textSize.width();
            alignPos.ry() -= l_sin * textSize.width();

            alignPos.rx() += l_sin * l_halfTextHeight - l_cos * spacing;
            alignPos.ry() -= l_cos * l_halfTextHeight + l_sin * spacing;
        }
        else // if ( align & Qt::AlignBottom )
        {
            alignPos.rx() += l_sin * l_halfTextHeight + l_cos * spacing;
            alignPos.ry() -= l_cos * l_halfTextHeight - l_sin * spacing;
        }
    }

    painter->translate( alignPos.x(), alignPos.y() );
    painter->rotate( angle );

    const QRectF textRect( 0, 0, textSize.width(), textSize.height() );
    label().draw( painter, textRect );
}
