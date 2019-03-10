#ifndef LQMEASSTUDIO_CHANNELSIGNALPARAMPANEL_H
#define LQMEASSTUDIO_CHANNELSIGNALPARAMPANEL_H

#include <QDockWidget>
#include "in/ChannelInfo.h"

class QTableWidget;
class QTableWidgetItem;


namespace LQMeasStudio {
    class ChannelInfo;
    class ChannelSignalParamPanel : public QDockWidget {
        Q_OBJECT
    public:
		explicit ChannelSignalParamPanel(QWidget *parent = nullptr);

    signals:

    public slots:
        int addParam(const QString& name);
        void enableParam(int id, bool enabled);
        void setParam(const QSharedPointer<ChannelInfo>&  ch, int id, double val, char fmt, int prec);


        void retranslate();
    private slots:

        void onChannelAppend(const QSharedPointer<ChannelInfo>& ch);
        void onChannelsClear();
        void onItemChanged(QTableWidgetItem* item);
        void onChannelVisibilityChanged(bool visible);



    private:
        static const int column_dev         = 0;
        static const int column_ch          = 1;
        static const int column_color       = 2;
        static const int column_par_first   = 3;

        QStringList stdColumnHdrs;
        QStringList paramsNames;


        QTableWidget* m_tbl;

        QVector<QSharedPointer<ChannelInfo> > m_channels;

        QStringList m_params;
    };

}
#endif // LQMEASSTUDIO_CHANNELSIGNALPARAMPANEL_H
