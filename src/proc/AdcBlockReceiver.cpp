#include "AdcBlockReceiver.h"
#include "LQMeasStudio.h"
#include "DataBlockProcessPlugin.h"
#include "lmath.h"
#include <QDebug>

namespace LQMeasStudio {
    AdcBlockReceiver::AdcBlockReceiver() : m_procBlockTime(25), m_procBlockInterval(1000) {

    }

    void AdcBlockReceiver::start() {
        foreach (AdcReceiver *receiver, LQMeasStudioEnv->inLauncher()->adcReceivers()) {
			connect(receiver, SIGNAL(dataReceived(QList<QSharedPointer<AdcReceiver::Data> > )),
					SLOT(processData(QList<QSharedPointer<AdcReceiver::Data> > )));
        }

        foreach (QSharedPointer<ChannelInfo> ch, LQMeasStudioEnv->channelsConfig()->channels()) {
            m_chProcInfo[ch->num()] = new ChProcInfo();
        }
    }

    void AdcBlockReceiver::stop() {
        foreach (AdcReceiver *receiver, LQMeasStudioEnv->inLauncher()->adcReceivers()) {
			disconnect(receiver, SIGNAL(dataReceived(QList<QSharedPointer<AdcReceiver::Data> > )),
					this, SLOT(processData(QList<QSharedPointer<AdcReceiver::Data> > )));

        }
    }

    void AdcBlockReceiver::clear() {
        qDeleteAll(m_chProcInfo);
        m_chProcInfo.clear();
    }

    void AdcBlockReceiver::setBlockTime(double blockTime, double intervalTime) {
        m_procBlockTime = blockTime;
        m_procBlockInterval = qMax(blockTime, intervalTime);
    }

	void AdcBlockReceiver::saveBlocks(QList<QSharedPointer<DataBlock> > &blockList) {
		if (LQMeasStudioEnv->savingDirectory().isEmpty()) {
			return;
		}

		QString filename = LQMeasStudioEnv->savingDirectory() + QDir::separator() + QDateTime::currentDateTime().toString("yyyy-MM-dd_hh-mm-ss-zzz") + QString(".oscbinz");

		QFile file(filename);
		if (file.open(QFile::WriteOnly)) {
			QByteArray binaryData;
			unsigned chCount = static_cast<unsigned>(blockList.length());
			binaryData.append(reinterpret_cast<char*>(&chCount), sizeof(chCount));
			for (unsigned ch = 0; ch < chCount; ++ch) {
				QSharedPointer<DataBlock> block = blockList[static_cast<signed>(ch)];
				if (block->channel()->device()->currentConfig()->adcChSoftAvg(ch)) {
					int points = 1;
					binaryData.append(reinterpret_cast<char*>(&points), sizeof(points));
					// apply averaging
					points = block->values().size();
					double sum = 0.;
					for (int p = 0; p < points; ++p) {
						sum += block->values().at(p);
					}
					sum /= points;
					float fv = static_cast<float>(sum);
					binaryData.append(reinterpret_cast<char*>(&fv), sizeof(fv));
				} else {
					int points = block->values().size();
//					qDebug() << "points" << points;
					binaryData.append(reinterpret_cast<char*>(&points), sizeof(points));
					for (int p = 0; p < points; ++p) {
						float value = static_cast<float>(block->values().at(p));
						binaryData.append(reinterpret_cast<char*>(&value), sizeof(value));
					}
//					qDebug() << "points written" << points;
				}
			}
			file.write(qCompress(binaryData));
			file.close();
		}
	}

	void AdcBlockReceiver::processData(QList<QSharedPointer<AdcReceiver::Data> > dataList) {
        QList<QSharedPointer<DataBlock> > blockList;

        foreach (QSharedPointer<AdcReceiver::Data> chData, dataList) {
             ChProcInfo *procInfo = m_chProcInfo[chData->ch->num()];
             LQMeas::DevAdc *adc = chData->ch->device()->devAdc();

             if (procInfo && adc) {
				unsigned show_pts = unsigned(m_procBlockTime / (1000.* chData->dt) + 0.5);
				unsigned intervalPts = unsigned(m_procBlockInterval / (1000. * chData->dt) + 0.5);
				unsigned procSize = 0;
				if (show_pts < 2) {
                    show_pts = 2;
				}

				while (procSize < static_cast<unsigned>(chData->data.size())) {
					if (!procInfo->blockSent) {
						// procInfo->y is the data
						// procInfo->startX is ??
						if (procInfo->procPoints < show_pts) {
							unsigned addSize = qMin((static_cast<unsigned>(chData->data.size()) - procSize),
													 (show_pts-procInfo->procPoints));
							if (procInfo->procPoints == 0) {
								procInfo->startX = chData->dt * (chData->x_idx + procSize) + chData->ch->timeShift();
                            }

							for (unsigned i=0; i < addSize; ++i) {
								procInfo->y.append(chData->data[static_cast<signed>(procSize + i)]);
                            }
							procSize += addSize;
							procInfo->procPoints += addSize;
                        }

						if (procInfo->procPoints >= show_pts) {
                            QSharedPointer<DataBlock> dataBlock =
								QSharedPointer<DataBlock>(new DataBlock(chData->ch, procInfo->y, procInfo->startX, chData->dt));
                            blockList.append(dataBlock);
							procInfo->blockSent = true;
                        }
                    }

					if (procInfo->blockSent) {
						if (procInfo->procPoints < intervalPts) {
							unsigned dropSize = qMin((static_cast<unsigned>(chData->data.size()) - procSize),
													 (intervalPts-procInfo->procPoints));
							procSize += dropSize;
							procInfo->procPoints+= dropSize;
                        }

						if (procInfo->procPoints >= intervalPts) {
							procInfo->procPoints = 0;
                            procInfo->y.clear();
							procInfo->blockSent = false;
                        }
                    }
                }
            }
        }

//		qDebug() << blockList.size();
		if (blockList.size()) {	// non-zero if data received, zero if the adc is idle
            foreach (QSharedPointer<DataBlock> block, blockList) {
                foreach (ProcessPlugin *plugin, LQMeasStudioEnv->processPlugings()) {
                    DataBlockProcessPlugin *blockPlugin = dynamic_cast<DataBlockProcessPlugin *>(plugin);
                    if (blockPlugin && blockPlugin->procEnabled()) {
                        blockPlugin->blockProcess(block);
						blockPlugin->blockShowResult(block);
                    }
                }
            }
			saveBlocks(blockList);
//			emit receiveBlock(blockList);
        }
    }
}

