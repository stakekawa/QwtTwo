/*!
 *  \file QwtTwoPlotMarker.h
 *
 *  \brief This file contains the declaration of QwtTwoPlotMarker class
 *  \author Stefano Takekawa s.takekawa_at_gmail.com
 *  \date 2017
 *  \copyright Apache Licence 2.0
 */


#ifndef QWTTWO_QWTTWOPLOTMARKER_MARKER_H
#define QWTTWO_QWTTWOPLOTMARKER_MARKER_H


#include <qwt_plot_marker.h>


/*!
 *  \brief A class for drawing markers with label rotated by an arbitrary angle
*/
class QWT_EXPORT QwtTwoPlotMarker : public QwtPlotMarker
{


public:

    /*!
     *  \brief Constructor
     */
    explicit QwtTwoPlotMarker(const QString& title = QString());

    /*!
     *  \brief Constructor
     */
    explicit QwtTwoPlotMarker(const QwtText& title);

    /*!
     *  \brief Destructor
     */
    virtual ~QwtTwoPlotMarker();

    /*!
     *   \brief Return label rotation angle in degrees
     */
    double labelRotation() const;

    /*!
     *  \brief Set label rotation angle in degrees
     */
    void setLabelRotation(const double);

    /*!
     *  \brief Draw the marker
     *
     *  \param painter Painter
     *  \param xMap x Scale Map
     *  \param yMap y Scale Map
     *  \param canvasRect Contents rectangle of the canvas in painter coordinates
     */
    void draw(QPainter*,
              const QwtScaleMap&,
              const QwtScaleMap&,
              const QRectF&) const override;


protected:

    /*!
     *  \brief Align and draw the text label of the marker applying the rotation angle
     *
     *  \param painter Painter
     *  \param canvasRect Contents rectangle of the canvas in painter coordinates
     *  \param pos Position of the marker, translated into widget coordinates
     *  \param correctionAngle Angle to applied due to xMap and yMap stretch
     */
    void drawLabel(QPainter*,
                   const QRectF&, const QPointF&,
                   const double&) const;


private:

    double m_labelRotation;      /*!< label rotation angle in degrees w.r.t. horizontal left to right writing used for computations - it breaks Qwt private implementation :-( */
    double m_labelRotationUser;  /*!< label rotation angle in degrees w.r.t. horizontal left to right writing set by the user - it breaks Qwt private implementation :-( */

    Q_DISABLE_COPY(QwtTwoPlotMarker)


};


#endif // QWTTWO_QWTTWOPLOTMARKER_MARKER_H
