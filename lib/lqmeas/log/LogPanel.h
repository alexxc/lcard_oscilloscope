#ifndef LTRSRVLOGPANEL_H
#define LTRSRVLOGPANEL_H

#include <QDockWidget>
#include <QTimer>

#include "LogModel.h"

#include <QPoint>

class QPushButton;
class QAction;
class QMenu;
class QFrame;
class QTableView;
class TableCopyAction;

namespace LQMeas {
    class LogPanel : public QDockWidget {
        Q_OBJECT
    public:
        explicit LogPanel(QWidget *parent = 0);
        ~LogPanel();

        QStringList logLevelNames();

    public slots:
        void clear(void);

        void resizeRows();
    protected:
        void changeEvent ( QEvent * event );
    private slots:
        void showContextMenu(QPoint point);
        void lvlActTriggered(QAction *act);
        void retranslateUi();

        void checkScroll();
        void onInserted(const QModelIndex &, int , int last);


    private:
        QPushButton *m_clearButton;
        QTableView *m_logbox;
        QAction *m_actClear, *m_actSelectAll, *m_actResize;
        TableCopyAction *m_actCopy;
        QMenu *m_contextMenu;
        QMenu *m_levelMenu;



        QHash<QAction*, LogLevel::Number> m_logLvlActMap;


        void showLogRec(LogMessage *entry);
        LogModel *m_model;
    };
}

#endif // LTRSRVLOGPANEL_H
