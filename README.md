# Общее описание #
**"L-Card Measurement Studio"** - это демонстрационная программа для модулей L502 и E502, позволяющая выполнять сбор данных АЦП, выводить простейшие сигналы на ЦАП (синус, постоянное значение, импульсы, пила), а также выполнять настройку сетевых параметров и обновление прошивки ARM (модуля E502)

Программа доступна как для ОС Windows, так и для ОС Linux:

- Сборки для Windows можно скачать со странички https://bitbucket.org/lcard/lqmeasstudio/downloads (достаточно распаковать архив, отдельного установщика на данный момент не предоставляется). Для работы программы также необходимо, чтобы была установленна последняя версия ["L-Card L502/E502 SDK"](http://www.lcard.ru/download/lpcie_setup.exe)
- Для ОС Linux предоставляются пакеты для поддерживаемых дистрибутивов. Пакет называется [lqmeasstudio](https://build.opensuse.org/package/show/home:l-card/lqmeasstudio). При установке пакетов все зависимости уже прописаны и должны установиться автоматически. Отдельно необходимо только устанавливать драйвер L502 (пакет lpcie-dkms).


Данная программа была разработана в первую очередь для внутренних нужд ООО "Л Кард" (в отличие от LGraph), поэтому выполняет лишь ограниченный набор функций, однако может быть полезна и конечному пользователю. Любые пожелания пользователей по возможностям приветствуются. Также возможно рассмотрение заказов на значительные доработки программы.

Ниже приведено основное окно программы:

![lqmeasstudio_main.png](https://bitbucket.org/repo/yAeq9E/images/801610445-lqmeasstudio_main.png "Основное окно программы")

Основное окно включает:

- меню с полным набором возможных действий
- панель инструментов для быстрого доступа к некоторым действиям
- основную область, предназначенную для отображения графиков
- дополнительные панели:
    * список устройств (см. ниже)
    * параметры сигналов - отображает список разрешенный каналов, цвет отображения и параметры каждого канала по последнему принятому блоку
    * журнал - отладочный вывод программы (скрыт по-умолчанию, но может понадобится при возникновении ошибок)

Дополнительные панели могут быть перемещены пользователем и привязаны к одной из сторон основного окна. Также каждая из них может быть скрыта или отображена (управляется из меню "Окна").


# Список устройств #
В списке устройств отображаются все найденные устройства в системе, которые поддерживает программа. По каждому устройству отображается полное его имя (включая опции) и серийный номер. Следует отметить, что данный список сам не обновляется, т.е. программа не отслеживает изменение подключения устройств. В случае если нужное устройство было подключено или отключено, необходимо нажать кнопку "Обновить список устройств", чтобы изменения вступили в силу. Также, если подключение устройства на время пропало, то для корректной работы с устройством следует обновить список устройств, т.к. в программе был открыто устройство, присутствующее до отключения.

В списке устройств можно отметить, с какими устройствами Вы собираетесь работать. Только для отмеченных устройств будет запущен сбор и/или генерация данных по соответствующей команде.

В списке устройств напротив названия каждого устройства с помощью иконки отображается интерфейс, по которому установлена связь с устройством.

Устройства, подключенные по интерфейсам USB и PCI/PCI Express находятся автоматически при нажатии кнопки "Обновить список устройств" (или при старте программы). При работе же с устройствами по Ethernet существует два способа подключения:

- по явно заданному IP-адресу
- по имени экземпляра автоматически обнаруженного устройства

Кроме того, для работы по Ethernet может понадобится настройка сетевых параметров устройства, как описано ниже в резделе **"Сетевые настройки"**.

## Подключение по явно заданному IP-адресу устройства.

Для этого необходимо зайти в меню "Устройства -> Настройка IP-адресов подключения" (или нажать на соответствующую иконку на панели инструментов). Появившейся диалог содержит список настроенный вручную IP-адресов устройств. 

![lqmeasstudio_ipaddr.png](https://bitbucket.org/repo/yAeq9E/images/119431471-lqmeasstudio_ipaddr.png "Настройка IP-адресов")
  
Если нужного IP-адреса нет в списке, то необходимо нажать кнопку "Добавить IP-адрес" и ввести требуемый адрес. После закрытия диалога по всем отмеченным адресам будет осуществлена попытка установить соединение с устройством и для каждого такого адреса в списке устройств появится соответствующая строка. При этом в скобках указывается IP-адрес, которому данное устройство соответствует. Если соединение по заданному IP-адресу установить не удалось, то соответствующая строка будет подсвечена красным.

![lqmeasstudio_ipaddr_dev.png](https://bitbucket.org/repo/yAeq9E/images/4175245465-lqmeasstudio_ipaddr_dev.png)
  
Список отмеченных адресов сохраняется и при последующем запуске программы или обновлении списка устройств будет выполнена попытка соединится по этим адресам.
  
## Подключение по имени экземпляра к автоматически обнаруженному устройству.

Данный способ действует только в локальной сети. Для подключения необходимо зайти в меню "Устройства -> просмотр устройств в локальной сети"  (или нажать на соответствующую иконку на панели инструментов). В появившемся окне отображаются все найденные в сети поддерживаемые модули. 

![lqmeasstudio_netbrowse.png](https://bitbucket.org/repo/yAeq9E/images/3366219196-lqmeasstudio_netbrowse.png)

При открытом диалоге список все время обновляется, т.к. если устройство подключится то будет добавлена строка, ему соответствующая. Следует отметить однако, что при отключении устройства (выключении питания или отключении кабеля) устройство исчезает из списка не мгновенно, т.к. модуль при таком отключении не может послать сообщение о отключении и запись о устройстве хранится некоторое время в кеше.

После закрытия окна будет установлено соединение со всеми отмеченными устройствами. Для каждого такого устройства в списке устройств появится соответствующая строка, при этом в скобках будет указано имя экземпляра. 

![lqmeasstudio_netsvcdev.png](https://bitbucket.org/repo/yAeq9E/images/736290343-lqmeasstudio_netsvcdev.png)

Программа в фоне отслеживает изменение присутствующих устройств в сети и, если устройство исчезнет, то исключает его и из списка текущих устройств.
  
Список выбранных устройств сохраняется (по именам экземпляров) и при следующем запуске программы выполняется ожидание обнаружения в сети устройств с этими именами и установка связи при их появлении. Если устройство не будет обнаружено в течение нескольких секунд после запуска программы, то оно исключается из списка подключения.
  
Поиск устройств в сети может быть явно запрещен (настраивается в окне со списком устройств в локальной сети). При этом удаляются и все записи о найденных в сети устройствах.


# Настройка параметров устройств #
Перед запуском сбора данных необходимо выполнить настройку нужных устройств. Для этого достаточно два раза нажать на названии устройства в списке устройств. В открывшемся окне, помимо настроек отображена информация о всех версиях прошивок модуля и наличии опций.
Настройки соответствуют параметрам, описанным в ["Руководстве программиста"](http://lcard.ru/download/x502api.pdf).

При нажатии кнопки "Применить" происходит настройка устройства и перерасчет частот ввода вывода (обновление значений до ближайших, поддерживаемых устройством) без закрытия окна настроек. 
По нажатии кнопки "Ок" происходит также настройка устройства и закрытие окна.

![lqmeasstudio_cfg.png](https://bitbucket.org/repo/yAeq9E/images/968628937-lqmeasstudio_cfg.png)

# Настройка сигналов ЦАП #
В окне настроек модуля можно настроить и сигналы, которые будут выводится синхронно на каждый канал ЦАП. Это может быть либо константа, либо один из периодических сигналов из заданного списка. 

Для генерации необходимо разрешить соответствующие каналы ЦАП. 
При нажатии на поле напротив разрешения канала (которое содержит описание настроенного сигнала) появится диалог настройки сигнала.

![lqmeasstudio_dac_sig.png](https://bitbucket.org/repo/yAeq9E/images/43271057-lqmeasstudio_dac_sig.png)

Параметры сигнала отличаются в зависимости от выбранного типа сигнала.
Помимо этого можно также выбрать предустанавливаемое значение. Это значение, которое будет выведено на ЦАП перед запуском синхронного ввода-вывода, т.е. оно определяет с какой точки начнется диаграмма при запуске синхронного вывода. Соответственно эта настройка влияет только на момент старта.

Сигналы для синхронной генерации устанавливаются один раз при настройке и для изменения необходимо остановить ввод/вывод, изменить настройки и запустить снова. Помимо этого на ЦАП можно выводить асинхронно постоянное значение, как описано ниже. При этом изменение этого значение может выполняться и при активном вводе/выводе.

# Прием данных #
При запуске сбора данных для всех выбранных устройств, для которых также был разрешен хотя бы один канал, запускается сбор данных. Хотя сбор данных ведется непрерывно, отображаются и обрабатываются данные по блокам. В настройках (меню "Файл -> Настройки приложения" или соответствующая иконка на панели инструментов) можно выбрать длительность одного блока, а также время от начала одного блока до начала другого. 

![lqmeasstudio_gen_settings.png](https://bitbucket.org/repo/yAeq9E/images/908921837-lqmeasstudio_gen_settings.png)

Этот блок выводится на временной график, а также, если разрешено в настройках, по всему блоку рассчитывается БПФ и строится спектр. Также по блоку рассчитывается  ряд параметров, значения которых отображаются в таблице на дополнительной панели "Параметры сигналов". Значение спектральных параметров соответствует описанному в статье Analog Devices ["Understand SINAD, ENOB, SNR, THD, THD + N, and SFDR"](http://www.analog.com/media/en/training-seminars/tutorials/MT-003.pdf). 

Также на панели "Параметры сигналов" отображается список разрешенных сигналов с указанием цветов на графике. Для каждого сигнала можно разрешить или запретить отображение.

Для изменения масштаба графиков можно нажать два раза левой кнопкой мыши и ввести вручную границы по осям графика.

![lqmeasstudio_plot_set.png](https://bitbucket.org/repo/yAeq9E/images/4004073013-lqmeasstudio_plot_set.png)

Следует отметить, что при выводе на графики не выполняется какое-либо прореживание сигналов, а также, если разрешено, спектр сигнала и его параметры рассчитываются по всем точкам блока. При больших блоках это может требовать существенных вычислительных затрат, поэтому пользователь должен отдавать себе отчет, что выбор больших блоков при недостаточном шаге между блоками может привести к полной загрузке ПК и программа может не успеть выполнить требуемые вычисления.

# Асинхронный вывод #
Программа позволяет асинхронно вывести постоянное значение на один из каналов ЦАП, сделать асинхронный ввод цифровых входов или вывод на цифровые линии. Для этого нужно нажать правой кнопкой на устройстве и выбрать в контекстном меню "Асинхронный ввод/вывод...".
По данному действию откроется следующего вида диалог

![lqmeasstudio_asyncpng.png](https://bitbucket.org/repo/yAeq9E/images/3316103271-lqmeasstudio_asyncpng.png)

Асинхронный ввод/вывод можно выполнять как при остановленном вводе-выводе, так и во время синхронного ввода-вывода. Однако асинхронный вывод на ЦАП доступен только для каналов, для которых не выполняется сейчас синхронный вывод.

Для вывода на ЦАП достаточно ввести для нужного канала значение в Вольтах, которое нужно установить, и нажать кнопку "Вывод на ЦАП1" или "Вывод на ЦАП2" для вывода на первый или второй канал соответственно.
Для вывода цифровых линий значения можно установить либо отдельно для каждой линии помощью нажатия на соответствующие светодиодные индикаторы (если горит зеленым, то будет выводится логическая "1", иначе - "0"), либо ввести шестнадцатеричный код, после чего по нажатию клавиши "Вывод" будут выведены указанные значения.
Для чтения значений цифровых линий достаточно нажать "Ввод" и текущие значения отобразятся как в виде шестнадцетиричного кода, так и состояния каждого входа на индикаторах в виде светодиодов (аналогично, если горит - значит на входе считано состояние логической "1").


# Сетевые настройки #
Для изменения сетевых настроек необходимо нажать правой кнопкой мыши на нужном устройстве и выбрать меню "Изменить настройки интерфейса...". Внести изменения, нажать "Ок" и ввести пароль (пустая строка, если не задан). Данный пункт доступен только для модулей E502 с наличием сетевого интерфейса. Значение настроек описаны в ["Руководстве программиста"](http://lcard.ru/download/x502api.pdf) в разделе "Особенности работы по интерфейсу Ethernet и настройка сетевых параметров".
При этом настройки можно изменять как в случае, если устройство подключено по USB, так и по Ethernet. Т.е. можно подключить устройство по USB, изменить его сетевые настройки, а затем уже включить в сеть и подключиться к нему по Ethernet.

![lqmeasstudio_ipcfg.png](https://bitbucket.org/repo/yAeq9E/images/239173977-lqmeasstudio_ipcfg.png "Диалог настройки сетевых параметров")



# Обновление прошивки ARM-контроллера Cortex-M4 #
Для модуля E502 программа позволяет выполнить обновление прошивки микроконтроллера. Последняя версия прошивки доступна для скачивания по следующему адресу: https://bitbucket.org/lcard/e502_m4/downloads. В названии файла указана версия прошивки. Текущая версия прошивки модуля отображается в диалоге настройки модуля. Рекомендуется всегда использовать последнюю версию прошивки микроконтроллера.

Для обновления достаточно скачать прошивку, после чего в программе нажать правой кнопкой по строке с устройством в списке устройств и выбрать во всплывающем меню пункт "Обновить прошивку..." и выбрать скачанный файл. В случае успешного обновления модуль будет автоматически перезагружен и готов к работе. 

![lqmeasstudio_lboot.png](https://bitbucket.org/repo/yAeq9E/images/478674226-lqmeasstudio_lboot.png)

Обновление может выполняться как по интерфейсу USB, так и по Ethernet.

Во время обновления прошивки модуль переводится в режим загрузчика (может потребовать установки драйвера по USB), о чем свидетельствует попеременное мигание красным-зеленым светодиода модуля. Если возникнет какая-либо ошибка, то модуль через 30 секунд восстановит старую прошивку и вернется в рабочий режим, поэтому достаточно просто подождать в течение этого времени.