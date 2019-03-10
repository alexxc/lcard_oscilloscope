#ifndef CONFIGDIALOGX502_H
#define CONFIGDIALOGX502_H

#include <QDialog>
#include <QMutex>
#include <QCheckBox>
#include "lqmeas/devs/x502/x502.h"
#include "lqmeas/devs/x502/x502Config.h"

namespace Ui {
class X502ConfigDialog;
}

class QTableWidget;
class QTableWidgetItem;
class QComboBox;
class QSpinBox;

class X502ConfigDialog : public QDialog {
    Q_OBJECT

public:
    explicit X502ConfigDialog(QWidget *parent, const QSharedPointer<LQMeas::x502>& dev);
    ~X502ConfigDialog();
public slots:
    void accept();
private Q_SLOTS:
    LQMeas::Error setConfig();
    void updateRefFreqElems(void);
    void adcMaxInChFreqChanged(int);
    void chModeChanged();
    void chRangeChanged();
	void chAvgChanged(int);
    void updateCommChState(int i);
    void cellDataChanged(int row, int col);
	void setMaxAdcFreq();
	void setMaxAdcChFreq();

    void allChEnChanged(int row, int col);
    void allChModeChanged();
    void allChRangeChanged();
	void allChAvgChanged(int val);

    void digLinesConfig();
    void addOption(const QString& text, bool val);
private:
    Ui::X502ConfigDialog *ui;

    void setComboBoxItemByData(QComboBox *box, int data);
    void fillSyncBox(QComboBox *box, int value);
    void addRangeItem(QComboBox *box, double val, unsigned code);
    void updateAllChState();

    struct ChUi {
        QTableWidgetItem *chItem;
        QComboBox *mode;
        QComboBox *range;
		QCheckBox  *avg;
    };

	void fillChUi(QTableWidget *table, unsigned int channel, ChUi *chUi, const QString& name, bool change_enabled);

    QSharedPointer<LQMeas::x502> m_dev;
    QSharedPointer<LQMeas::x502Config> m_cfg;
    QList<ChUi> m_chUis;
    ChUi m_allChUi{};
    //volatile bool m_allUpdated;
    QMutex m_allUpMutex;
};

#endif // CONFIGDIALOGX502
