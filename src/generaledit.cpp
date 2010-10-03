#include "generaledit.h"
#include "ui_generaledit.h"
#include "pers.h"
#include "helpers.h"
#include <QtGui>

#define BIT_WARN_THR     ( 0x01 )
#define BIT_WARN_SW      ( 0x02 )
#define BIT_WARN_MEM     ( 0x04 )
#define BIT_WARN_MEM     ( 0x04 )
#define BIT_BEEP_VAL     ( 0x38 ) // >>3
#define BEEP_VAL_SHIFT   3

GeneralEdit::GeneralEdit(EEPFILE *eFile, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GeneralEdit)
{
    ui->setupUi(this);
    this->setWindowIcon(QIcon(":/icon.ico"));
    eeFile = eFile;

    if(!eeFile->eeLoadGeneral())  eeFile->generalDefault();
    eeFile->getGeneralSettings(&g_eeGeneral);

    QSettings settings("er9x-eePe", "eePe");
    ui->tabWidget->setCurrentIndex(settings.value("generalEditTab", 0).toInt());

    populateSwitchCB(ui->backlightswCB,g_eeGeneral.lightSw);

    ui->contrastSB->setValue(g_eeGeneral.contrast);
    ui->battwarningDSB->setValue((double)g_eeGeneral.vBatWarn/10);
    ui->battcalibDSB->setValue((double)g_eeGeneral.vBatCalib/10);
    ui->battCalib->setValue((double)g_eeGeneral.vBatCalib/10);
    ui->backlightautoSB->setValue(g_eeGeneral.lightAutoOff*5);
    ui->inactimerSB->setValue(g_eeGeneral.inactivityTimer);
    ui->thrrevChkB->setChecked(g_eeGeneral.throttleReversed);
    ui->inputfilterCB->setCurrentIndex(g_eeGeneral.filterInput);
    ui->thrwarnChkB->setChecked(!(g_eeGeneral.warnOpts & BIT_WARN_THR));   //Default is zero=checked
    ui->switchwarnChkB->setChecked(!(g_eeGeneral.warnOpts & BIT_WARN_SW)); //Default is zero=checked
    ui->memwarnChkB->setChecked(!(g_eeGeneral.warnOpts & BIT_WARN_MEM));   //Default is zero=checked
    ui->beeperCB->setCurrentIndex((g_eeGeneral.warnOpts & BIT_BEEP_VAL) >> BEEP_VAL_SHIFT);
    ui->channelorderCB->setCurrentIndex(g_eeGeneral.templateSetup);
    ui->stickmodeCB->setCurrentIndex(g_eeGeneral.stickMode);

    ui->ana1Neg->setValue(g_eeGeneral.calibSpanNeg[0]);
    ui->ana2Neg->setValue(g_eeGeneral.calibSpanNeg[1]);
    ui->ana3Neg->setValue(g_eeGeneral.calibSpanNeg[2]);
    ui->ana4Neg->setValue(g_eeGeneral.calibSpanNeg[3]);
    ui->ana5Neg->setValue(g_eeGeneral.calibSpanNeg[4]);
    ui->ana6Neg->setValue(g_eeGeneral.calibSpanNeg[5]);
    ui->ana7Neg->setValue(g_eeGeneral.calibSpanNeg[6]);

    ui->ana1Mid->setValue(g_eeGeneral.calibMid[0]);
    ui->ana2Mid->setValue(g_eeGeneral.calibMid[1]);
    ui->ana3Mid->setValue(g_eeGeneral.calibMid[2]);
    ui->ana4Mid->setValue(g_eeGeneral.calibMid[3]);
    ui->ana5Mid->setValue(g_eeGeneral.calibMid[4]);
    ui->ana6Mid->setValue(g_eeGeneral.calibMid[5]);
    ui->ana7Mid->setValue(g_eeGeneral.calibMid[6]);

    ui->ana1Pos->setValue(g_eeGeneral.calibSpanPos[0]);
    ui->ana2Pos->setValue(g_eeGeneral.calibSpanPos[1]);
    ui->ana3Pos->setValue(g_eeGeneral.calibSpanPos[2]);
    ui->ana4Pos->setValue(g_eeGeneral.calibSpanPos[3]);
    ui->ana5Pos->setValue(g_eeGeneral.calibSpanPos[4]);
    ui->ana6Pos->setValue(g_eeGeneral.calibSpanPos[5]);
    ui->ana7Pos->setValue(g_eeGeneral.calibSpanPos[6]);

    ui->PPM1->setValue(g_eeGeneral.ppmInCalib[0]);
    ui->PPM2->setValue(g_eeGeneral.ppmInCalib[1]);
    ui->PPM3->setValue(g_eeGeneral.ppmInCalib[2]);
    ui->PPM4->setValue(g_eeGeneral.ppmInCalib[3]);
    ui->PPM5->setValue(g_eeGeneral.ppmInCalib[4]);
    ui->PPM6->setValue(g_eeGeneral.ppmInCalib[5]);
    ui->PPM7->setValue(g_eeGeneral.ppmInCalib[6]);
    ui->PPM8->setValue(g_eeGeneral.ppmInCalib[7]);

}

GeneralEdit::~GeneralEdit()
{
    delete ui;
}

void GeneralEdit::updateSettings()
{
    int16_t sum=0;
    for(int i=0; i<12;i++) sum+=g_eeGeneral.calibMid[i];
    g_eeGeneral.chkSum = sum;
    eeFile->putGeneralSettings(&g_eeGeneral);

    emit modelValuesChanged();
}

void GeneralEdit::on_contrastSB_editingFinished()
{
    g_eeGeneral.contrast = ui->contrastSB->value();
    updateSettings();
}

void GeneralEdit::on_battwarningDSB_editingFinished()
{
    g_eeGeneral.vBatWarn = (int)(ui->battwarningDSB->value()*10);
    updateSettings();
}

void GeneralEdit::on_battcalibDSB_editingFinished()
{
    g_eeGeneral.vBatCalib = ui->battcalibDSB->value()*10;
    ui->battCalib->setValue(ui->battcalibDSB->value());
    updateSettings();
}

void GeneralEdit::on_backlightswCB_currentIndexChanged(int index)
{
    g_eeGeneral.lightSw = index-MAX_DRSWITCH;
    updateSettings();
}

void GeneralEdit::on_backlightautoSB_editingFinished()
{
    int i = ui->backlightautoSB->value()/5;
    if((i*5)!=ui->backlightautoSB->value())
        ui->backlightautoSB->setValue(i*5);
    else
    {
        g_eeGeneral.lightAutoOff = i;
        updateSettings();
    }
}

void GeneralEdit::on_inactimerSB_editingFinished()
{
    g_eeGeneral.inactivityTimer = ui->inactimerSB->value();
    updateSettings();
}

void GeneralEdit::on_thrrevChkB_stateChanged(int )
{
    g_eeGeneral.throttleReversed = ui->thrrevChkB->isChecked() ? 1 : 0;
    updateSettings();
}

void GeneralEdit::on_inputfilterCB_currentIndexChanged(int index)
{
    g_eeGeneral.filterInput = index;
    updateSettings();
}

void GeneralEdit::on_thrwarnChkB_stateChanged(int )
{
    if(!ui->thrwarnChkB->isChecked()) //default value is 0 => checked!
        g_eeGeneral.warnOpts |= BIT_WARN_THR;
    else
        g_eeGeneral.warnOpts &= ~BIT_WARN_THR;

    updateSettings();
}

void GeneralEdit::on_switchwarnChkB_stateChanged(int )
{
    if(!ui->switchwarnChkB->isChecked()) //default value is 0 => checked!
        g_eeGeneral.warnOpts |= BIT_WARN_SW;
    else
        g_eeGeneral.warnOpts &= ~BIT_WARN_SW;

    updateSettings();
}

void GeneralEdit::on_memwarnChkB_stateChanged(int )
{
    if(!ui->memwarnChkB->isChecked()) //default value is 0 => checked!
        g_eeGeneral.warnOpts |= BIT_WARN_MEM;
    else
        g_eeGeneral.warnOpts &= ~BIT_WARN_MEM;

    updateSettings();
}

void GeneralEdit::on_beeperCB_currentIndexChanged(int index)
{
    g_eeGeneral.warnOpts = (g_eeGeneral.warnOpts & ~BIT_BEEP_VAL) | index << BEEP_VAL_SHIFT;
    updateSettings();
}

void GeneralEdit::on_channelorderCB_currentIndexChanged(int index)
{
    g_eeGeneral.templateSetup = index;
    updateSettings();
}

void GeneralEdit::on_stickmodeCB_currentIndexChanged(int index)
{
    g_eeGeneral.stickMode = index;
    updateSettings();
}



void GeneralEdit::on_ana1Neg_editingFinished()
{
    g_eeGeneral.calibSpanNeg[0] = ui->ana1Neg->value();
    updateSettings();
}

void GeneralEdit::on_ana2Neg_editingFinished()
{
    g_eeGeneral.calibSpanNeg[1] = ui->ana2Neg->value();
    updateSettings();
}

void GeneralEdit::on_ana3Neg_editingFinished()
{
    g_eeGeneral.calibSpanNeg[2] = ui->ana3Neg->value();
    updateSettings();
}

void GeneralEdit::on_ana4Neg_editingFinished()
{
    g_eeGeneral.calibSpanNeg[3] = ui->ana4Neg->value();
    updateSettings();
}

void GeneralEdit::on_ana5Neg_editingFinished()
{
    g_eeGeneral.calibSpanNeg[4] = ui->ana5Neg->value();
    updateSettings();
}

void GeneralEdit::on_ana6Neg_editingFinished()
{
    g_eeGeneral.calibSpanNeg[5] = ui->ana6Neg->value();
    updateSettings();
}

void GeneralEdit::on_ana7Neg_editingFinished()
{
    g_eeGeneral.calibSpanNeg[6] = ui->ana7Neg->value();
    updateSettings();
}



void GeneralEdit::on_ana1Mid_editingFinished()
{
    g_eeGeneral.calibMid[0] = ui->ana1Mid->value();
    updateSettings();
}

void GeneralEdit::on_ana2Mid_editingFinished()
{
    g_eeGeneral.calibMid[1] = ui->ana2Mid->value();
    updateSettings();
}

void GeneralEdit::on_ana3Mid_editingFinished()
{
    g_eeGeneral.calibMid[2] = ui->ana3Mid->value();
    updateSettings();
}

void GeneralEdit::on_ana4Mid_editingFinished()
{
    g_eeGeneral.calibMid[3] = ui->ana4Mid->value();
    updateSettings();
}

void GeneralEdit::on_ana5Mid_editingFinished()
{
    g_eeGeneral.calibMid[4] = ui->ana5Mid->value();
    updateSettings();
}

void GeneralEdit::on_ana6Mid_editingFinished()
{
    g_eeGeneral.calibMid[5] = ui->ana6Mid->value();
    updateSettings();
}

void GeneralEdit::on_ana7Mid_editingFinished()
{
    g_eeGeneral.calibMid[6] = ui->ana7Mid->value();
    updateSettings();
}



void GeneralEdit::on_ana1Pos_editingFinished()
{
    g_eeGeneral.calibSpanPos[0] = ui->ana1Pos->value();
    updateSettings();
}

void GeneralEdit::on_ana2Pos_editingFinished()
{
    g_eeGeneral.calibSpanPos[1] = ui->ana2Pos->value();
    updateSettings();
}

void GeneralEdit::on_ana3Pos_editingFinished()
{
    g_eeGeneral.calibSpanPos[2] = ui->ana3Pos->value();
    updateSettings();
}

void GeneralEdit::on_ana4Pos_editingFinished()
{
    g_eeGeneral.calibSpanPos[3] = ui->ana4Pos->value();
    updateSettings();
}

void GeneralEdit::on_ana5Pos_editingFinished()
{
    g_eeGeneral.calibSpanNeg[4] = ui->ana5Pos->value();
    updateSettings();
}

void GeneralEdit::on_ana6Pos_editingFinished()
{
    g_eeGeneral.calibSpanNeg[5] = ui->ana6Pos->value();
    updateSettings();
}

void GeneralEdit::on_ana7Pos_editingFinished()
{
    g_eeGeneral.calibSpanPos[6] = ui->ana7Pos->value();
    updateSettings();
}

void GeneralEdit::on_battCalib_editingFinished()
{
    g_eeGeneral.vBatCalib = ui->battCalib->value()*10;
    ui->battcalibDSB->setValue(ui->battCalib->value());
    updateSettings();
}

void GeneralEdit::on_PPM1_editingFinished()
{
    g_eeGeneral.ppmInCalib[0] = ui->PPM1->value();
    updateSettings();
}

void GeneralEdit::on_PPM2_editingFinished()
{
    g_eeGeneral.ppmInCalib[1] = ui->PPM2->value();
    updateSettings();
}

void GeneralEdit::on_PPM3_editingFinished()
{
    g_eeGeneral.ppmInCalib[2] = ui->PPM3->value();
    updateSettings();
}

void GeneralEdit::on_PPM4_editingFinished()
{
    g_eeGeneral.ppmInCalib[3] = ui->PPM4->value();
    updateSettings();
}

void GeneralEdit::on_PPM5_editingFinished()
{
    g_eeGeneral.ppmInCalib[4] = ui->PPM5->value();
    updateSettings();
}

void GeneralEdit::on_PPM6_editingFinished()
{
    g_eeGeneral.ppmInCalib[5] = ui->PPM6->value();
    updateSettings();
}

void GeneralEdit::on_PPM7_editingFinished()
{
    g_eeGeneral.ppmInCalib[6] = ui->PPM7->value();
    updateSettings();
}

void GeneralEdit::on_PPM8_editingFinished()
{
    g_eeGeneral.ppmInCalib[7] = ui->PPM8->value();
    updateSettings();
}

void GeneralEdit::on_tabWidget_currentChanged(int index)
{
    QSettings settings("er9x-eePe", "eePe");
    settings.setValue("generalEditTab",index);//ui->tabWidget->currentIndex());
}
