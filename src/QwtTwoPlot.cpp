/*!
 *  \file QwtTwoPlot.h
 *
 *  \brief This file contains the declaration of QwtTwoPlot class
 *  \author Stefano Takekawa s.takekawa_at_gmail.com
 *  \date 2017
 *  \copyright Apache Licence 2.0
 */


#include <memory>

#include <QApplication>
#include <QCursor>
#include <QLabel>
#include <QMouseEvent>
#include <QResizeEvent>
#include <qwt_plot_curve.h>
#include <qwt_plot_grid.h>
#include <qwt_plot_magnifier.h>
#include <qwt_plot_panner.h>
#include <qwt_plot_rescaler.h>
#include <qwt_scale_engine.h>
#include <qwt_scale_div.h>
#include <qwt_scale_widget.h>
#include "QtTwo/QDpiHelper.h"
#include "QtTwo/QtTwoDefines.h"
#include "QtTwo/QtTwoUtils.h"
#include "QwtTwoPlot.h"


/*                                                                            */
/* Private data                                                               */
/*                                                                            */


static constexpr double k_xAxisMinDefault    = 650.;
static const     double k_xAxisMaxDefault    = 850.;
static constexpr double k_yAxisMinDefault    = 100.;
static constexpr double k_yAxisMaxDefault    = 300.;
static constexpr double k_gridMajorPenFactor =   3.;
static constexpr double k_gridMinorPenFactor =   3.;
static constexpr int    k_gridMajorMaxTicks  =    8;
static constexpr int    k_gridMinorMaxTicks  =    0;
static constexpr float  k_plotMargin         = 10.f;
static constexpr float  k_marginNormal       = 10.f;

static const QColor     k_gridPenColor = QColor(0x40, 0x40, 0x40);


/*                                                                            */
/* Public data                                                                */
/*                                                                            */

/* explicit */
QwtTwoPlot::QwtTwoPlot(QWidget* p_parent /* = Q_NULLPTR */) :
    QwtPlot(p_parent),
    m_title(NULL),
    m_grid(NULL),
    m_magnifier(NULL),
    m_panner(NULL),
    m_xAxisMin(k_xAxisMinDefault),
    m_xAxisMax(k_xAxisMaxDefault),
    m_yAxisMin(k_yAxisMinDefault),
    m_yAxisMax(k_yAxisMaxDefault),
    m_xAxisInverted(false),
    m_yAxisInverted(false),
    m_mouseTracking(false),
    m_restorePanner(true)
{
    const int l_contentMargin = static_cast<int>(k_plotMargin * QDpiHelper::aspectRatio());

    QwtPlot::setContentsMargins(l_contentMargin, l_contentMargin, l_contentMargin, l_contentMargin);
    QwtTwoPlot::configurePlot();

    QTTWO_CONNECT4(axisWidget(QwtPlot::xBottom), SIGNAL(scaleDivChanged()), this, SLOT(axisXScaleChanged()));
    QTTWO_CONNECT4(axisWidget(QwtPlot::xTop   ), SIGNAL(scaleDivChanged()), this, SLOT(axisXScaleChanged()));
    QTTWO_CONNECT4(axisWidget(QwtPlot::yLeft  ), SIGNAL(scaleDivChanged()), this, SLOT(axisYScaleChanged()));
    QTTWO_CONNECT4(axisWidget(QwtPlot::yRight ), SIGNAL(scaleDivChanged()), this, SLOT(axisYScaleChanged()));
}


/* virtual */
QwtTwoPlot::~QwtTwoPlot()
{
    deleteWidget(m_title);
}


void QwtTwoPlot::setFontSize(const int p_fontSize)
{
    QFont l_font = QwtPlot::font();
    l_font.setPixelSize(p_fontSize);
    QwtPlot::setFont(l_font);

    l_font = QwtPlot::title().font();
    l_font.setPixelSize(p_fontSize);
    QwtPlot::title().setFont(l_font);

    l_font = QwtPlot::axisTitle(QwtPlot::xTop).font();
    l_font.setPixelSize(p_fontSize);

    for (int i=0; i<QwtPlot::axisCnt; ++i)
    {
        QwtPlot::axisWidget(i)->setFont(l_font);
    }

    if (NULL != m_title)
    {
        l_font = m_title->font();
        l_font.setPixelSize(p_fontSize);
        m_title->setFont(l_font);

        centerTitleLabel();
    }
}


void QwtTwoPlot::setPalette(const QPalette& p_palette)
{
    QPalette l_palette = p_palette;

    l_palette.setBrush(QPalette::Base,        p_palette.brush(QPalette::Window     ));
    l_palette.setBrush(QPalette::Window,      p_palette.brush(QPalette::Window     ));
    l_palette.setBrush(QPalette::WindowText,  p_palette.brush(QPalette::WindowText ));
    l_palette.setBrush(QPalette::Text,        p_palette.brush(QPalette::Text       ));
    l_palette.setBrush(QPalette::ToolTipBase, p_palette.brush(QPalette::ToolTipBase));
    l_palette.setBrush(QPalette::ToolTipText, p_palette.brush(QPalette::ToolTipText));
    l_palette.setBrush(QPalette::Button,      p_palette.brush(QPalette::Window     ));
    l_palette.setBrush(QPalette::ButtonText,  p_palette.brush(QPalette::ButtonText ));
    l_palette.setBrush(QPalette::BrightText,  p_palette.brush(QPalette::BrightText ));

    /* 3D effects */

    l_palette.setBrush(QPalette::Light,    p_palette.brush(QPalette::Window));
    l_palette.setBrush(QPalette::Midlight, p_palette.brush(QPalette::Window));
    l_palette.setBrush(QPalette::Dark,     p_palette.brush(QPalette::Window));
    l_palette.setBrush(QPalette::Mid,      p_palette.brush(QPalette::Window));
    l_palette.setBrush(QPalette::Shadow,   p_palette.brush(QPalette::Window));

    /* selected appeareance */

    l_palette.setBrush(QPalette::Highlight,       p_palette.brush(QPalette::Highlight      ));
    l_palette.setBrush(QPalette::HighlightedText, p_palette.brush(QPalette::HighlightedText));

    /* disabled stuff */

    l_palette.setBrush(QPalette::Disabled, QPalette::Text,       p_palette.brush(QPalette::Disabled, QPalette::Text      ));
    l_palette.setBrush(QPalette::Disabled, QPalette::WindowText, p_palette.brush(QPalette::Disabled, QPalette::WindowText));
    l_palette.setBrush(QPalette::Disabled, QPalette::Button,     p_palette.brush(QPalette::Disabled, QPalette::Window    ));
    l_palette.setBrush(QPalette::Disabled, QPalette::ButtonText, p_palette.brush(QPalette::Disabled, QPalette::ButtonText));

    QwtPlot::setPalette(l_palette);

    QwtPlot::setPalette(l_palette);
    QwtPlot::canvas()->setPalette(l_palette);

    for (int i=0; i<QwtPlot::axisCnt; ++i)
    {
        QwtPlot::axisWidget(i)->setPalette(l_palette);
    }
}


void QwtTwoPlot::axisScaleX(double& p_xAxisMin, double& p_xAxisMax)
{
    const double l_lowerBound = axisScaleDiv(QwtPlot::xBottom).lowerBound();
    const double l_upperBound = axisScaleDiv(QwtPlot::xBottom).upperBound();

    p_xAxisMin = std::min(l_lowerBound, l_upperBound);
    p_xAxisMax = std::max(l_lowerBound, l_upperBound);
}


void QwtTwoPlot::axisScaleY(double& p_yAxisMin, double& p_yAxisMax)
{
    const double l_lowerBound = axisScaleDiv(QwtPlot::yLeft).lowerBound();
    const double l_upperBound = axisScaleDiv(QwtPlot::yLeft).upperBound();

    p_yAxisMin = std::min(l_lowerBound, l_upperBound);
    p_yAxisMax = std::max(l_lowerBound, l_upperBound);
}


/*                                                                            */
/* Protected methods                                                          */
/*                                                                            */


void QwtTwoPlot::enablePanner()
{
    if (NULL != m_panner)
    {
        delete m_panner;
    }

    m_panner = new QwtPlotPanner(canvas());
}


void QwtTwoPlot::disablePanner()
{
    if (NULL != m_panner)
    {
        delete m_panner;
        m_panner = NULL;
    }
}


void QwtTwoPlot::enableMagnifier()
{
    if (NULL != m_magnifier)
    {
        delete m_magnifier;
    }

    m_magnifier = new QwtPlotMagnifier(canvas());
    m_magnifier->setMouseButton(Qt::NoButton); /* disable the modifier so nothing interferes with QMenu right clicks */
}


void QwtTwoPlot::disableMagnifier()
{
    if (NULL != m_magnifier)
    {
        delete m_magnifier;
        m_magnifier = NULL;
    }
}


void QwtTwoPlot::disableMagnifierAxisX()
{
    if (NULL != m_magnifier)
    {
        delete m_magnifier;
    }

    m_magnifier = new QwtPlotMagnifier(canvas());
    m_magnifier->setMouseButton(Qt::NoButton); /* disable the modifier so nothing interferes with QMenu right clicks */
    m_magnifier->setAxisEnabled(QwtPlot::xBottom, false);
    m_magnifier->setAxisEnabled(QwtPlot::xTop,    false);
}


void QwtTwoPlot::disableMagnifierAxisY()
{
    if (NULL != m_magnifier)
    {
        delete m_magnifier;
    }

    m_magnifier = new QwtPlotMagnifier(canvas());
    m_magnifier->setMouseButton(Qt::NoButton); /* disable the modifier so nothing interferes with QMenu right clicks */
    m_magnifier->setAxisEnabled(QwtPlot::yLeft,  false);
    m_magnifier->setAxisEnabled(QwtPlot::yRight, false);
}


void QwtTwoPlot::setAxisScaleX(const double p_min, const double p_max)
{
    if ((false == std::isnan(p_min)) &&
        (false == std::isnan(p_max)))
    {
        if (false == m_xAxisInverted)
        {
            QwtPlot::setAxisScale(QwtPlot::xTop,    p_min, p_max);
            QwtPlot::setAxisScale(QwtPlot::xBottom, p_min, p_max);
        }
        else
        {
            QwtPlot::setAxisScale(QwtPlot::xTop,    p_max, p_min);
            QwtPlot::setAxisScale(QwtPlot::xBottom, p_max, p_min);
        }

        m_xAxisMin = std::min(p_min, p_max);
        m_xAxisMax = std::max(p_min, p_max);

        QwtPlot::replot();
    }
}


void QwtTwoPlot::setAxisScaleY(const double p_min, const double p_max)
{
    if ((false == std::isnan(p_min)) &&
        (false == std::isnan(p_max)))
    {
        if (false == m_yAxisInverted)
        {
            QwtPlot::setAxisScale(QwtPlot::yLeft,  p_min, p_max);
            QwtPlot::setAxisScale(QwtPlot::yRight, p_min, p_max);
        }
        else
        {
            QwtPlot::setAxisScale(QwtPlot::yLeft,  p_max, p_min);
            QwtPlot::setAxisScale(QwtPlot::yRight, p_max, p_min);
        }

        m_yAxisMin = std::min(p_min, p_max);
        m_yAxisMax = std::max(p_min, p_max);

        QwtPlot::replot();
    }
}


void QwtTwoPlot::setAxisInvertedX(const bool p_inverted)
{
    m_xAxisInverted = p_inverted;

    QwtPlot::axisScaleEngine(QwtPlot::xTop   )->setAttribute(QwtScaleEngine::Inverted, p_inverted);
    QwtPlot::axisScaleEngine(QwtPlot::xBottom)->setAttribute(QwtScaleEngine::Inverted, p_inverted);

    const double l_lowerBound = axisScaleDiv(QwtPlot::xBottom).lowerBound();
    const double l_upperBound = axisScaleDiv(QwtPlot::xBottom).upperBound();

    if ((false == std::isnan(l_lowerBound)) &&
        (false == std::isnan(l_upperBound)))
    {
        setAxisScaleX(l_lowerBound, l_upperBound);
    }
}


void QwtTwoPlot::setAxisInvertedY(const bool p_inverted)
{
    m_yAxisInverted = p_inverted;

    QwtPlot::axisScaleEngine(QwtPlot::yLeft )->setAttribute(QwtScaleEngine::Inverted, p_inverted);
    QwtPlot::axisScaleEngine(QwtPlot::yRight)->setAttribute(QwtScaleEngine::Inverted, p_inverted);

    const double l_lowerBound = axisScaleDiv(QwtPlot::yLeft).lowerBound();
    const double l_upperBound = axisScaleDiv(QwtPlot::yLeft).upperBound();

    if ((false == std::isnan(l_lowerBound)) &&
        (false == std::isnan(l_upperBound)))
    {
        setAxisScaleY(l_lowerBound, l_upperBound);
    }
}


void QwtTwoPlot::resetAxisScale()
{
    setAxisScaleX(m_xAxisMin, m_xAxisMax);
    setAxisScaleY(m_yAxisMin, m_yAxisMax);
}


void QwtTwoPlot::storeAxisScale()
{
    if (false == m_xAxisInverted)
    {
        m_xAxisMin = QwtPlot::axisScaleDiv(QwtPlot::xBottom).lowerBound();
        m_xAxisMax = QwtPlot::axisScaleDiv(QwtPlot::xBottom).upperBound();
    }
    else
    {
        m_xAxisMax = QwtPlot::axisScaleDiv(QwtPlot::xBottom).lowerBound();
        m_xAxisMin = QwtPlot::axisScaleDiv(QwtPlot::xBottom).upperBound();
    }

    if (false == m_yAxisInverted)
    {
        m_yAxisMin = QwtPlot::axisScaleDiv(QwtPlot::yLeft).lowerBound();
        m_yAxisMax = QwtPlot::axisScaleDiv(QwtPlot::yLeft).upperBound();
    }
    else
    {
        m_yAxisMax = QwtPlot::axisScaleDiv(QwtPlot::yLeft).lowerBound();
        m_yAxisMin = QwtPlot::axisScaleDiv(QwtPlot::yLeft).upperBound();
    }
}


void QwtTwoPlot::setAxisTitleX(const QString& p_title /* = QString("") */)
{
    QwtPlot::setAxisTitle(QwtPlot::xTop,    p_title);
    QwtPlot::setAxisTitle(QwtPlot::xBottom, p_title);
}


void QwtTwoPlot::setAxisTitleY(const QString& p_title /* = QString("") */)
{
    QwtPlot::setAxisTitle(QwtPlot::yLeft,  p_title);
    QwtPlot::setAxisTitle(QwtPlot::yRight, p_title);
}


void QwtTwoPlot::setTitle(const QString& p_title /* = QString("") */)
{
    if (true == p_title.isEmpty())
    {
        deleteWidget(m_title);
    }
    else
    {
        if (NULL == m_title)
        {
            m_title = new QLabel(canvas());
            m_title->setAlignment(Qt::AlignCenter);
            m_title->setAttribute(Qt::WA_TransparentForMouseEvents);
        }

        m_title->setText(QString("<b>") + p_title + QString("</b>"));

        centerTitleLabel();
    }
}


void QwtTwoPlot::setTitleFontSize(const float p_fontSize)
{
    if (NULL != m_title)
    {
        QFont l_font = m_title->font();
        l_font.setPointSize(static_cast<int>(p_fontSize));
        m_title->setFont(l_font);
    }
}


void QwtTwoPlot::showAxisX(const bool p_show)
{
    QwtPlot::enableAxis(QwtPlot::xBottom, p_show);
}


void QwtTwoPlot::showAxisY(const bool p_show)
{
    QwtPlot::enableAxis(QwtPlot::yLeft, p_show);
}


void QwtTwoPlot::setAxisSameAspectRatio()
{
    QwtPlotRescaler* l_rescaler = new QwtPlotRescaler(canvas());

    l_rescaler->setRescalePolicy(QwtPlotRescaler::Expanding);
    l_rescaler->setExpandingDirection(QwtPlotRescaler::ExpandBoth);

    double l_xMin = 0.;
    double l_xMax = 0.;
    double l_yMin = 0.;
    double l_yMax = 0.;

    axisScaleX(l_xMin, l_xMax);
    axisScaleY(l_yMin, l_yMax);

    const double l_xRange = fabs(l_xMin - l_xMax);
    const double l_yRange = fabs(l_yMin - l_yMax);

    if (l_xRange < l_yRange)
    {
        l_rescaler->setReferenceAxis(QwtPlot::yLeft);
    }
    else
    {
        l_rescaler->setReferenceAxis(QwtPlot::xBottom);
    }

    l_rescaler->setAspectRatio(1.);

    l_rescaler->rescale();

    delete l_rescaler;

    QwtPlot::replot();
}


void QwtTwoPlot::enableMouseTracking(const bool p_enable, const bool p_disablePanner)
{
    m_mouseTracking = p_enable;

    canvas()->setAttribute(Qt::WA_MouseTracking, p_enable);

    if (true == p_enable)
    {
        m_restorePanner = (NULL != m_panner);

        if (true == p_disablePanner)
        {
            disablePanner();
        }

    }
    else if ((true == m_restorePanner) &&
             (false == p_disablePanner))
    {
        enablePanner();
    }
    else
    {
        disablePanner();
    }

    if (false == p_enable)
    {
        QApplication::restoreOverrideCursor();
    }
}


/*                                                                            */
/* Private slots                                                              */
/*                                                                            */


void QwtTwoPlot::onAxisXScaleChanged()
{
    if (false == m_xAxisInverted)
    {
        emit axisScaleXChanged(QwtPlot::axisScaleDiv(QwtPlot::xBottom).lowerBound(), QwtPlot::axisScaleDiv(QwtPlot::xBottom).upperBound());
    }
    else
    {
        emit axisScaleXChanged(QwtPlot::axisScaleDiv(QwtPlot::xBottom).upperBound(), QwtPlot::axisScaleDiv(QwtPlot::xBottom).lowerBound());
    }
}


void QwtTwoPlot::onAxisYScaleChanged()
{
    if (false == m_yAxisInverted)
    {
        emit axisScaleYChanged(QwtPlot::axisScaleDiv(QwtPlot::yLeft).lowerBound(), QwtPlot::axisScaleDiv(QwtPlot::yLeft).upperBound());
    }
    else
    {
        emit axisScaleYChanged(QwtPlot::axisScaleDiv(QwtPlot::yLeft).upperBound(), QwtPlot::axisScaleDiv(QwtPlot::yLeft).lowerBound());
    }
}


/*                                                                            */
/* Protected methos                                                           */
/*                                                                            */


/* virtual */
void QwtTwoPlot::enterEvent(QEvent*)
{
    if (true == m_mouseTracking)
    {
        QApplication::setOverrideCursor(QCursor(QPixmap(":/profile_checker/crosshair-red.png")));
    }
}


/* virtual */
void QwtTwoPlot::leaveEvent(QEvent*)
{
    if (true == m_mouseTracking)
    {
        QApplication::restoreOverrideCursor();
    }
}


/* virtual */
void QwtTwoPlot::resizeEvent(QResizeEvent* p_event)
{
    QwtPlot::resizeEvent(p_event);

    centerTitleLabel();
}


/* virtual */
void QwtTwoPlot::mouseMoveEvent(QMouseEvent* p_event)
{
    if (NULL != p_event)
    {
        const QPoint& l_mousePos = p_event->pos();

        /* signal mouse row position */

        emit mousePosition(l_mousePos);

        /* signal mouse position w.r.t. axis scale */

        double l_x = 0.;
        double l_y = 0.;

        if (true == mousePositionToPlotPosition(p_event, l_x, l_y))
        {
            emit mousePositionOnPlot(l_x, l_y);
        }
    }
}


/* virtual */
void QwtTwoPlot::mousePressEvent(QMouseEvent* p_event)
{
    if (NULL != p_event)
    {
        const QPoint& l_mousePos = p_event->pos();

        /* signal mouse row position */

        emit mousePressed(l_mousePos);

        /* signal mouse position w.r.t. axis scale */

        double l_x = 0.;
        double l_y = 0.;

        if (true == mousePositionToPlotPosition(p_event, l_x, l_y))
        {
            emit mousePressedOnPlot(l_x, l_y);
        }
    }
}


/* virtual */
void QwtTwoPlot::mouseReleaseEvent(QMouseEvent* p_event)
{
    onAxisXScaleChanged();
    onAxisYScaleChanged();

    if (NULL != p_event)
    {
        const QPoint& l_mousePos = p_event->pos();

        /* signal mouse row position */

        emit mouseReleased(l_mousePos);

        /* signal mouse position w.r.t. axis scale */

        double l_x = 0.;
        double l_y = 0.;

        if (true == mousePositionToPlotPosition(p_event, l_x, l_y))
        {
            emit mouseReleasedOnPlot(l_x, l_y);
        }
    }
}


/*                                                                            */
/* Private members                                                            */
/*                                                                            */


void QwtTwoPlot::configurePlot()
{
    /* axis stuff */

    showAxisX(true);
    showAxisY(true);

    setAxisScaleX(m_xAxisMin, m_xAxisMax);
    setAxisScaleY(m_yAxisMin, m_yAxisMax);

    /* grid */

    m_grid = new QwtPlotGrid();
    m_grid->enableXMin(true);
    m_grid->enableYMin(true);

    m_grid->setMajorPen(k_gridPenColor, m_grid->majorPen().widthF() / k_gridMajorPenFactor, Qt::DashDotLine   );
    m_grid->setMinorPen(k_gridPenColor, m_grid->majorPen().widthF() / k_gridMinorPenFactor, Qt::DashDotDotLine);
    QwtPlot::setAxisMaxMajor(QwtPlot::xBottom, k_gridMajorMaxTicks);
    QwtPlot::setAxisMaxMinor(QwtPlot::xBottom, k_gridMinorMaxTicks);
    QwtPlot::setAxisMaxMajor(QwtPlot::yLeft,   k_gridMajorMaxTicks);
    QwtPlot::setAxisMaxMinor(QwtPlot::yLeft,   k_gridMinorMaxTicks);

    m_grid->attach(this);

    /* magnifier */

    enableMagnifier();

    /* panner */

    enablePanner();
}


void QwtTwoPlot::centerTitleLabel()
{
    if (NULL != m_title)
    {
        m_title->adjustSize();

        if (NULL != canvas())
        {
            const QSize l_canvasSize = canvas()->size();

            const float l_xPos = static_cast<float>(l_canvasSize.width() - m_title->width()) / 2.f;
            const float l_yPos = k_marginNormal;

            m_title->QTTWO_MOVE(l_xPos, l_yPos);
        }
    }
}


bool QwtTwoPlot::mousePositionToPlotPosition(const QMouseEvent* p_event, double& p_x, double& p_y)
{
    bool l_returnComputed = true;

    if (NULL == p_event)
    {
        l_returnComputed = false;
    }
    else if (NULL == canvas())
    {
        l_returnComputed = false;
    }
    else
    {

#if QT_VERSION < 0x050000
        const qreal l_xMouseOnWidget = p_event->posF().x() - canvas()->pos().rx();
        const qreal l_yMouseOnWidget = p_event->posF().y() - canvas()->pos().ry();
#else
        const qreal l_xMouseOnWidget = p_event->localPos().x() - canvas()->pos().rx();
        const qreal l_yMouseOnWidget = p_event->localPos().y() - canvas()->pos().ry();
#endif

        p_x = canvasMap(QwtPlot::xBottom).invTransform(l_xMouseOnWidget);
        p_y = canvasMap(QwtPlot::yLeft  ).invTransform(l_yMouseOnWidget);

    } /* end if (NULL == p_event) */

    return l_returnComputed;
}
