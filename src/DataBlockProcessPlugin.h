#ifndef DATABLOCKPROCESSPLUGIN_H
#define DATABLOCKPROCESSPLUGIN_H

#include "ProcessPlugin.h"
#include "DataBlock.h"

namespace LQMeasStudio {

    /* Базовый класс для плагина обработки блоков данных АЦП.
       При приеме блока данных в отдельном потоке выполняется проход по всем
       плагинам данного типа, вызывая blockProcess().
       Затем данный блок передается в основной поток, где для
       всех плагинов вызывается blockShowResult() или blockDropResult() */
    class DataBlockProcessPlugin : public ProcessPlugin {
        Q_OBJECT
    public:
		DataBlockProcessPlugin(QString name, bool optional = false, QObject *parent = nullptr) :
			ProcessPlugin(name, optional, parent) {}

        /* Метод выполняется в отдельном потоке обработки блоков.
           Должна выполнять рассчет требуемых параметров и сохранение их
           в контексте блока данных через DataBlock::setProperty() */
        virtual void blockProcess(QSharedPointer<DataBlock> block) = 0;
        /* Метод выполняется в основном потоке приложения и служит для
           отображения рассчитанных в blockProcess() параметров,
           которые должны быть получены из контекста данных через
           DataBlock::property() */
        virtual void blockShowResult(QSharedPointer<DataBlock> block) = 0;
        /* Метод должен быть реализован, если сохраненные в blockProcess()
           результаты должны быть явно осовобождены. Вызывается вместо
           blockShowResult() в случае, если обновление вывода запрещено */
		virtual void blockDropResult(QSharedPointer<DataBlock> block) {Q_UNUSED(block)}


    private:

    };
}

#endif // DATABLOCKESSPLUGIN_H

