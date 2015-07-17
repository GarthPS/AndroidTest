#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    if (!QGeoPositionInfoSource::availableSources().isEmpty()) {
        ui->label_info->setText("Positionnement actif");
        createSource();
    } else {
        ui->label_info->setText("Désole, le positonnement n'est pas disponible sur votre plateforme.");
    }

}

void MainWindow::createSource(){
    source = QGeoPositionInfoSource::createDefaultSource(this);
    if (source) {
        connect(source, SIGNAL(positionUpdated(QGeoPositionInfo)), this, SLOT(positionUpdated(QGeoPositionInfo)));
        connect(source, SIGNAL(error(QGeoPositionInfoSource::Error)), this, SLOT(error(QGeoPositionInfoSource::Error)));
        connect(source, SIGNAL(updateTimeout()), this, SLOT(missingFix()));
        source->startUpdates();
        qDebug() << "minimumUpdateInterval" << source->minimumUpdateInterval();
        qDebug() << "updateInterval" << source->updateInterval();
        source->setUpdateInterval(1);
        qDebug() << "minimumUpdateInterval" << source->minimumUpdateInterval();
        qDebug() << "updateInterval" << source->updateInterval();
    }

    satelliteInfo = QGeoSatelliteInfoSource::createDefaultSource(this);
    if (satelliteInfo) {
        connect(satelliteInfo, SIGNAL(satellitesInViewUpdated(QList<QGeoSatelliteInfo>)), this, SLOT(updateSatInView(QList<QGeoSatelliteInfo>)));
        connect(satelliteInfo, SIGNAL(satellitesInUseUpdated(QList<QGeoSatelliteInfo>)), this, SLOT(updateSatInUse(QList<QGeoSatelliteInfo>)));
        satelliteInfo->startUpdates();
    }

    elaspedTime.start();
}


void MainWindow::positionUpdated(const QGeoPositionInfo &info)
{
        ui->lcdDirection->display(info.attribute(QGeoPositionInfo::Direction));
        ui->lcdGNDSpeed->display(info.attribute(QGeoPositionInfo::GroundSpeed)*3.6);
        ui->lcdHDOP->display(info.attribute(QGeoPositionInfo::HorizontalAccuracy));
        ui->dateTimeEdit->setDateTime(info.timestamp());
        ui->label_coord->setText(info.coordinate().toString(QGeoCoordinate::DegreesMinutesSecondsWithHemisphere));
        qDebug() << "Position updated:" << info.hasAttribute(QGeoPositionInfo::VerticalAccuracy)<< info.hasAttribute(QGeoPositionInfo::MagneticVariation)<< info.hasAttribute(QGeoPositionInfo::VerticalSpeed);
        qDebug() << "Position updated:" << QString::number(elaspedTime.elapsed());
        elaspedTime.restart();
}


void MainWindow::updateSatInView(QList<QGeoSatelliteInfo> inView)
{
    satInView = QString::number(inView.count());
    ui->label_info->setText(satInUse+"/"+satInView);
}


void MainWindow::updateSatInUse(QList<QGeoSatelliteInfo> inUse)
{
    satInUse = QString::number(inUse.count());
    ui->label_info->setText(satInUse+"/"+satInView);
}


void MainWindow::error(QGeoPositionInfoSource::Error positioningError)
{
    if (QGeoPositionInfoSource::ClosedError == positioningError) {
        ui->label_info->setText("Veuillez activer le positionnement");// todo: maj info
    }
    else {
        ui->label_info->setText("Erreur inconnue. à analyser");
    }
}


void MainWindow::missingFix()
{
    ui->label_info->setText(ui->label_info->text()+"Positionnement momentanément indisponible.");
}


MainWindow::~MainWindow()
{
    delete ui;
}
