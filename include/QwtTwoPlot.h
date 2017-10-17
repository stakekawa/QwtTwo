/*!
 *  \file QwtTwoPlot.h
 *
 *  \brief This file contains the declaration of QwtTwoPlot class
 *  \author Stefano Takekawa s.takekawa_at_gmail.com
 *  \date 2017
 *  \copyright Apache Licence 2.0
 */


#ifndef QTTWO_QWTTWOPLOT_H
#define QTTWO_QWTTWOPLOT_H


#include <qwt_plot.h>


class QwtPlotGrid;
class QwtPlotMagnifier;
class QwtPlotPanner;
class QLabel;


class QWT_EXPORT QwtTwoPlot : public QwtPlot
{
    Q_OBJECT


public:

    /*!
     *  \brief Constructor
     */
    explicit QwtTwoPlot(QWidget* p_parent = Q_NULLPTR);

    /*!
     *  \brief Destructor
     */
    virtual ~QwtTwoPlot();

    bool axisInvertedX() const { return m_xAxisInverted; }

    bool axisInvertedY() const { return m_yAxisInverted; }

    void setFontSize(const int p_fontSize);

    void setPalette(const QPalette& p_palette);

    void axisScaleX(double& p_xAxisMin, double& p_xAxisMax);

    void axisScaleY(double& p_yAxisMin, double& p_yAxisMax);


public slots:

    /*!
     *  \brief Enable the panner
     */
    void enablePanner();

    /*!
     *  \brief Disable the panner
     */
    void disablePanner();

    /*!
     *  \brief Enable the magnifier on both axis
     */
    void enableMagnifier();

    /*!
     *  \brief Disable the magnifier
     */
    void disableMagnifier();

    /*!
     *  \brief Disable the magnifier on x axis
     */
    void disableMagnifierAxisX();

    /*!
     *  \brief Disable the magnifier on y axis
     */
    void disableMagnifierAxisY();

    /*!
     *  \brief Set scale on x axis, taking into account if the x axis is inverted or not
     */
    void setAxisScaleX(const double p_min, const double p_max);

    /*!
     *  \brief Set scale on y axis, taking into account if the y axis is inverted or not
     */
    void setAxisScaleY(const double p_min, const double p_max);

    /*!
     *  \brief Set x axis inverted
     */
    void setAxisInvertedX(const bool p_inverted);

    /*!
     *  \brief Set y axis inverted
     */
    void setAxisInvertedY(const bool p_inverted);

    /*!
     *  \brief Reset both x and y axis scale with last set using SetAxisScaleX() and SetAxisScaleY()
     */
    void resetAxisScale();

    /*!
     *  \brief Store both x and y axis scale ranges
     */
    void storeAxisScale();

    /*!
     *  \brief Configure the x axis title
     */
    void setAxisTitleX(const QString& p_title = QString());

    /*!
     *  \brief Configure the y axis title
     */
    void setAxisTitleY(const QString& p_title = QString());

    /*!
     *  \brief Configure the plot title
     */
    void setTitle(const QString& p_title = QString());

    /*!
     *  \brief Configure the font size of QwtTwoPlot
     */
    void setTitleFontSize(const float p_fontSize);

    /*!
     *  \brief Show x axis
     */
    void showAxisX(const bool p_show);

    /*!
     *  \brief Show y axis
     */
    void showAxisY(const bool p_show);

    /*!
     *  \brief Set the same aspect ratio on both axis
     */
    void setAxisSameAspectRatio();

    /*!
     *  \brief Enable Mouse Tracking
     */
    void enableMouseTracking(const bool p_enable, const bool p_disablePanner);


private slots:

    void onAxisXScaleChanged();

    void onAxisYScaleChanged();


signals:

    void axisScaleXChanged(const double p_lowerBound, const double p_upperBound);

    void axisScaleYChanged(const double p_lowerBound, const double p_upperBound);

    void mousePressed(const QPoint& p_position);

    void mousePressedOnPlot(const double p_x, const double p_y);

    void mouseReleased(const QPoint& p_position);

    void mouseReleasedOnPlot(const double p_x, const double p_y);

    void mousePosition(const QPoint& p_position);

    void mousePositionOnPlot(const double p_x, const double p_y);


protected:

    void enterEvent(QEvent*) override;

    void leaveEvent(QEvent*) override;

    void resizeEvent(QResizeEvent* p_event) override;

    void mouseMoveEvent(QMouseEvent* p_event) override;

    void mousePressEvent(QMouseEvent* p_event) override;

    void mouseReleaseEvent(QMouseEvent* p_event) override;


private:

    /*!
     *  \brief Configure the QwtPlot
     */
    void configurePlot();

    /*!
     *  \brief Center the title label on the QwtPlot canvas
     */
    void centerTitleLabel();

    /*!
     *  \brief Compute the mouse position x, y values from mouse position on QwtPlot widget (pixels)
     *
     *  \param p_event
     *  \param p_x
     *  \param p_z
     *
     *  \return true if success otherwise false
     */
    bool mousePositionToPlotPosition(const QMouseEvent* p_event, double& p_x, double& p_y);

    QLabel*                          m_title;

    QwtPlotGrid*                     m_grid;
    QwtPlotMagnifier*                m_magnifier;
    QwtPlotPanner*                   m_panner;

    double                           m_xAxisMin;
    double                           m_xAxisMax;
    double                           m_yAxisMin;
    double                           m_yAxisMax;

    bool                             m_xAxisInverted;
    bool                             m_yAxisInverted;

    bool                             m_mouseTracking;
    bool                             m_restorePanner;

    Q_DISABLE_COPY(QwtTwoPlot)


};





#endif // QTTWO_QWTTWOPLOT_H
