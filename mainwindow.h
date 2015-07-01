#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGeoCoordinate>
#include <QGeoPositionInfo>
#include <QGeoPositionInfoSource>
#include <QGeoSatelliteInfoSource>
#include <QElapsedTimer>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    QGeoPositionInfoSource *source;
    QGeoSatelliteInfoSource *satelliteInfo;
    QElapsedTimer elaspedTime;
    Ui::MainWindow *ui;
    QString satInView;
    QString satInUse;

private slots:
    void positionUpdated(const QGeoPositionInfo &info);
    void error(QGeoPositionInfoSource::Error positioningError);
    void missingFix();
    void createSource();
    void updateSatInView(QList<QGeoSatelliteInfo> inView);
    void updateSatInUse(QList<QGeoSatelliteInfo> inUse);
};

#endif // MAINWINDOW_H
