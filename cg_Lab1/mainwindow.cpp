#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDesktopWidget>
#include <QScreen>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->labelError->setVisible(false);

    colourModel = new ColourModel(this);

    connect(colourModel, SIGNAL(colourChanged(QString)), this, SLOT(colourModelChanged(QString)));

    // rgb
    connect(ui->sliderRGBr, SIGNAL(sliderMoved(int)), this, SLOT(rgbSliderMoved()));
    connect(ui->sliderRGBg, SIGNAL(sliderMoved(int)), this, SLOT(rgbSliderMoved()));
    connect(ui->sliderRGBb, SIGNAL(sliderMoved(int)), this, SLOT(rgbSliderMoved()));
    connect(ui->sBoxRGBr, SIGNAL(editingFinished()), this, SLOT(rgbSBoxChanged()));
    connect(ui->sBoxRGBg, SIGNAL(editingFinished()), this, SLOT(rgbSBoxChanged()));
    connect(ui->sBoxRGBb, SIGNAL(editingFinished()), this, SLOT(rgbSBoxChanged()));

    // hsv
    connect(ui->sliderHSVh, SIGNAL(sliderMoved(int)), this, SLOT(hsvSliderMoved()));
    connect(ui->sliderHSVs, SIGNAL(sliderMoved(int)), this, SLOT(hsvSliderMoved()));
    connect(ui->sliderHSVv, SIGNAL(sliderMoved(int)), this, SLOT(hsvSliderMoved()));
    connect(ui->sBoxHSVh, SIGNAL(editingFinished()), this, SLOT(hsvSBoxChanged()));
    connect(ui->sBoxHSVs, SIGNAL(editingFinished()), this, SLOT(hsvSBoxChanged()));
    connect(ui->sBoxHSVv, SIGNAL(editingFinished()), this, SLOT(hsvSBoxChanged()));

    // cmyk
    connect(ui->sliderCMYKc, SIGNAL(sliderMoved(int)), this, SLOT(cmykSliderMoved()));
    connect(ui->sliderCMYKm, SIGNAL(sliderMoved(int)), this, SLOT(cmykSliderMoved()));
    connect(ui->sliderCMYKy, SIGNAL(sliderMoved(int)), this, SLOT(cmykSliderMoved()));
    connect(ui->sliderCMYKk, SIGNAL(sliderMoved(int)), this, SLOT(cmykSliderMoved()));
    connect(ui->sBoxCMYKc, SIGNAL(editingFinished()), this, SLOT(cmykSBoxChanged()));
    connect(ui->sBoxCMYKm, SIGNAL(editingFinished()), this, SLOT(cmykSBoxChanged()));
    connect(ui->sBoxCMYKy, SIGNAL(editingFinished()), this, SLOT(cmykSBoxChanged()));
    connect(ui->sBoxCMYKk, SIGNAL(editingFinished()), this, SLOT(cmykSBoxChanged()));

    // luv
    connect(ui->sliderLUVl, SIGNAL(sliderMoved(int)), this, SLOT(luvSliderMoved()));
    connect(ui->sliderLUVu, SIGNAL(sliderMoved(int)), this, SLOT(luvSliderMoved()));
    connect(ui->sliderLUVv, SIGNAL(sliderMoved(int)), this, SLOT(luvSliderMoved()));
    connect(ui->sBoxLUVl, SIGNAL(editingFinished()), this, SLOT(luvSBoxChanged()));
    connect(ui->sBoxLUVu, SIGNAL(editingFinished()), this, SLOT(luvSBoxChanged()));
    connect(ui->sBoxLUVv, SIGNAL(editingFinished()), this, SLOT(luvSBoxChanged()));

    colourModelChanged(QString());
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::changeBackGroundColourFromModel()
{
    QPalette pal(palette());
    QColor backgroundColour(colourModel->getRGB().r, colourModel->getRGB().g,
                            colourModel->getRGB().b);
    pal.setColor(QPalette::Background, backgroundColour);
    ui->widget->setPalette(pal);
}

void MainWindow::updateRGBfromModel()
{
    ui->sliderRGBr->setValue(colourModel->getRGB().r);
    ui->sliderRGBg->setValue(colourModel->getRGB().g);
    ui->sliderRGBb->setValue(colourModel->getRGB().b);

    ui->sBoxRGBr->setValue(colourModel->getRGB().r);
    ui->sBoxRGBg->setValue(colourModel->getRGB().g);
    ui->sBoxRGBb->setValue(colourModel->getRGB().b);
}

void MainWindow::updateHSVfromModel()
{
    ui->sliderHSVh->setValue(colourModel->getHSV().h);
    ui->sliderHSVs->setValue(colourModel->getHSV().s * 100);
    ui->sliderHSVv->setValue(colourModel->getHSV().v * 100);

    ui->sBoxHSVh->setValue(colourModel->getHSV().h);
    ui->sBoxHSVs->setValue(colourModel->getHSV().s);
    ui->sBoxHSVv->setValue(colourModel->getHSV().v);
}

void MainWindow::updateCMYKfromModel()
{
    ui->sliderCMYKc->setValue(colourModel->getCMYK().c);
    ui->sliderCMYKm->setValue(colourModel->getCMYK().m);
    ui->sliderCMYKy->setValue(colourModel->getCMYK().y);
    ui->sliderCMYKk->setValue(colourModel->getCMYK().k);

    ui->sBoxCMYKc->setValue(colourModel->getCMYK().c);
    ui->sBoxCMYKm->setValue(colourModel->getCMYK().m);
    ui->sBoxCMYKy->setValue(colourModel->getCMYK().y);
    ui->sBoxCMYKk->setValue(colourModel->getCMYK().k);
}

void MainWindow::updateLUVfromModel()
{
    ui->sliderLUVl->setValue(colourModel->getLUV().l);
    ui->sliderLUVu->setValue(colourModel->getLUV().u);
    ui->sliderLUVv->setValue(colourModel->getLUV().v);

    ui->sBoxLUVl->setValue(colourModel->getLUV().l);
    ui->sBoxLUVu->setValue(colourModel->getLUV().u);
    ui->sBoxLUVv->setValue(colourModel->getLUV().v);
}

void MainWindow::rgbSliderMoved()
{
    colourModel->setColourInRGB(ui->sliderRGBr->sliderPosition(), ui->sliderRGBg->sliderPosition(),
                                ui->sliderRGBb->sliderPosition());
}

void MainWindow::rgbSBoxChanged()
{
    colourModel->setColourInRGB(ui->sBoxRGBr->value(), ui->sBoxRGBg->value(),
                                ui->sBoxRGBb->value());
    ui->sliderRGBr->setSliderPosition(ui->sBoxRGBr->value());
    ui->sliderRGBg->setSliderPosition(ui->sBoxRGBg->value());
    ui->sliderRGBb->setSliderPosition(ui->sBoxRGBb->value());
}

void MainWindow::hsvSliderMoved()
{
    colourModel->setColourInHSV(ui->sliderHSVh->sliderPosition(),
                                ui->sliderHSVs->sliderPosition() / 100.,
                                ui->sliderHSVv->sliderPosition() / 100.);
    ui->sBoxHSVs->setValue(ui->sliderHSVs->sliderPosition() / 100.);
    ui->sBoxHSVv->setValue(ui->sliderHSVv->sliderPosition() / 100.);
}

void MainWindow::hsvSBoxChanged()
{
    colourModel->setColourInHSV(ui->sBoxHSVh->value(), ui->sBoxHSVs->value(),
                                ui->sBoxHSVv->value());
    ui->sliderHSVh->setSliderPosition(ui->sBoxHSVh->value());
    ui->sliderHSVs->setSliderPosition(ui->sBoxHSVs->value() * 100);
    ui->sliderHSVv->setSliderPosition(ui->sBoxHSVv->value() * 100);
}

void MainWindow::cmykSliderMoved()
{
    colourModel->setColourInCMYK(ui->sliderCMYKc->value(), ui->sliderCMYKm->value(),
                                 ui->sliderCMYKy->value(), ui->sliderCMYKk->value());
}

void MainWindow::cmykSBoxChanged()
{
    colourModel->setColourInCMYK(ui->sBoxCMYKc->value(), ui->sBoxCMYKm->value(),
                                 ui->sBoxCMYKy->value(), ui->sBoxCMYKk->value());
    ui->sliderCMYKc->setSliderPosition(ui->sBoxCMYKc->value());
    ui->sliderCMYKm->setSliderPosition(ui->sBoxCMYKm->value());
    ui->sliderCMYKy->setSliderPosition(ui->sBoxCMYKy->value());
    ui->sliderCMYKk->setSliderPosition(ui->sBoxCMYKk->value());
}

void MainWindow::luvSliderMoved()
{
    colourModel->setColourInLUV(ui->sliderLUVl->value(), ui->sliderLUVu->value(),
                                ui->sliderLUVv->value());
}

void MainWindow::luvSBoxChanged()
{

    colourModel->setColourInLUV(ui->sBoxLUVl->value(), ui->sBoxLUVu->value(),
                                ui->sBoxLUVv->value());
    ui->sliderLUVl->setSliderPosition(ui->sBoxLUVl->value());
    ui->sliderLUVu->setSliderPosition(ui->sBoxLUVu->value());
    ui->sliderLUVv->setSliderPosition(ui->sBoxLUVv->value());
}

void MainWindow::colourModelChanged(QString _in_model)
{
    changeBackGroundColourFromModel();

    if (_in_model != "ERROR") {
        ui->labelError->setVisible(false);
        if (_in_model != "RGB")
            updateRGBfromModel();
        if (_in_model != "HSV")
            updateHSVfromModel();
        if (_in_model != "CMYK")
            updateCMYKfromModel();
        if (_in_model != "LUV")
            updateLUVfromModel();
    } else
        ui->labelError->setVisible(true);
}

void MainWindow::on_btnPalette_clicked()
{
    QPixmap pixmap = QGuiApplication::primaryScreen()->grabWindow(ui->btnPalette->winId());
    QPoint point = ui->btnPalette->mapFromGlobal(QCursor::pos());
    QColor colour = QColor::fromRgb(pixmap.toImage().pixel(point));
    colourModel->setColourInRGB(colour.red(), colour.green(), colour.blue());
    updateRGBfromModel();
}
