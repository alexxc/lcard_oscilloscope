#include "ChannelSignalParamPanel.h"
#include <QColorDialog>
#include <QHeaderView>
#include <QLocale>
#include <QTableWidget>
#include "actions/TableCopyAction.h"
#include "LQMeasStudio.h"

namespace LQMeasStudio {
    ChannelSignalParamPanel::ChannelSignalParamPanel(QWidget *parent) :
        QDockWidget(parent) {


        setObjectName("SignalParamsPanel");


        m_tbl = new QTableWidget(this);
        m_tbl->addAction(new TableCopyAction(m_tbl));

        //m_tbl->horizontalHeader()->setStretchLastSection(true);
        m_tbl->verticalHeader()->setVisible(false);
        m_tbl->setEditTriggers(QAbstractItemView::NoEditTriggers);


        retranslate();

        //connect(m_tbl, SIGNAL(itemDoubleClicked(QTableWidgetItem*)), SLOT(on_itemDoubleClicked(QTableWidgetItem*)));
        //connect(m_tbl, SIGNAL(itemChanged(QTableWidgetItem*)), SLOT(on_itemChanged(QTableWidgetItem*)));


        connect(LQMeasStudioEnv->channelsConfig(), SIGNAL(append(QSharedPointer<ChannelInfo>)),
                SLOT(onChannelAppend(QSharedPointer<ChannelInfo>)));
        connect(LQMeasStudioEnv->channelsConfig(), SIGNAL(cleared()),
                SLOT(onChannelsClear()));

        connect(m_tbl, SIGNAL(itemChanged(QTableWidgetItem*)), SLOT(onItemChanged(QTableWidgetItem*)));


        setWidget(m_tbl);
    }

    int ChannelSignalParamPanel::addParam(const QString& name) {
        paramsNames.append(name);
        m_tbl->setColumnCount(stdColumnHdrs.size() + paramsNames.size());
        m_tbl->setHorizontalHeaderLabels(stdColumnHdrs + paramsNames);
        return paramsNames.size()-1;
    }

    void ChannelSignalParamPanel::enableParam(int id, bool enabled) {
        int col = id + column_par_first;
        m_tbl->setColumnHidden(col, !enabled);
    }

    void ChannelSignalParamPanel::setParam(const QSharedPointer<ChannelInfo>& ch, int id, double val, char fmt, int prec) {
        /** @todo возможно правильнее отдельный список каналов с указанием столбцов,
         *  на случай, если номер будут не всегда по порядку */

        int row = ch->num();
        int col = id + column_par_first;

        if ((col < m_tbl->columnCount()) && (row < m_tbl->rowCount())) {
            QLocale locale;
            m_tbl->item(row, col)->setText(locale.toString(val, fmt, prec));
        }
    }

    void ChannelSignalParamPanel::retranslate() {
        setWindowTitle(tr("Signal parameters"));
        stdColumnHdrs.clear();
        stdColumnHdrs << tr("Device") << tr("Channel") << tr("Color");

        m_tbl->setColumnCount(stdColumnHdrs.size() + paramsNames.size());
        m_tbl->setHorizontalHeaderLabels(stdColumnHdrs + paramsNames);
    }

    void ChannelSignalParamPanel::onChannelAppend(const QSharedPointer<ChannelInfo>& ch) {
        int row = m_tbl->rowCount();
        m_channels.append(ch);
        m_tbl->setRowCount(row+1);
        m_tbl->setItem(row, column_dev, new QTableWidgetItem(ch->device()->devStr()));

        QTableWidgetItem* ch_item=new QTableWidgetItem(ch->name());
        ch_item->setFlags(ch_item->flags() | Qt::ItemIsUserCheckable | Qt::ItemIsTristate);
        ch_item->setCheckState(ch->visible() ? Qt::Checked : Qt::Unchecked);
        m_tbl->setItem(row, column_ch, ch_item);

        ch_item = new QTableWidgetItem();
        ch_item->setFlags(Qt::ItemIsEnabled);
        ch_item->setBackgroundColor(ch->color());
        m_tbl->setItem(row, column_color, ch_item);

        for (int col = column_par_first;  col < m_tbl->columnCount(); col++) {
            m_tbl->setItem(row, col, new QTableWidgetItem());
        }

        connect(ch.data(), SIGNAL(visibilityChanged(bool)), SLOT(onChannelVisibilityChanged(bool)));
    }

    void ChannelSignalParamPanel::onChannelsClear() {
        foreach (QSharedPointer<ChannelInfo> ch, m_channels) {
            disconnect(ch.data(), SIGNAL(visibilityChanged(bool)), this, SLOT(onChannelVisibilityChanged(bool)));
        }
        m_channels.clear();
        m_tbl->setRowCount(0);
    }

    void ChannelSignalParamPanel::onItemChanged(QTableWidgetItem *item) {
        if (item->column()==column_ch) {
            bool val = item->checkState()==Qt::Checked;
            if (item->row() < m_channels.size()) {
                m_channels[item->row()]->setVisible(val);
            }
        }
    }

    void ChannelSignalParamPanel::onChannelVisibilityChanged(bool visible) {
        auto *info = qobject_cast<ChannelInfo*>(sender());
        if (info) {
            int idx = info->num();
            if (idx < m_channels.size()) {
                m_tbl->item(idx, column_ch)->setCheckState(visible ? Qt::Checked : Qt::Unchecked);
            }
        }
    }
}
