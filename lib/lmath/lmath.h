#ifndef LTEST_MATH_H
#define LTEST_MATH_H

/** Коды ошибок */
typedef enum {
    LMATH_ERR_INVALID_WINTYPE = -20000, /**< Неверно задан тип окна */
    LMATH_ERR_MEMORY_ALLOC    = -20001, /**< Ошибка выделения памяти */
    LMATH_ERR_INVALID_PARAMS  = -20002, /**< Неверно заданы входные параметры */
    LMATH_ERR_INTERNAL_CALC   = -20003  /**< Ошибка при вычислениях */
} t_lmath_errs;


/** Параметры используемого окна. Заполняются при вызове функций
 *  lmath_window()/lmath_scaled_window() и используются другими
 *  функциями при дальнейшей обработке */
typedef struct {
    double inherent_gain; /**< Сумма коэф. окна в квадрате / size */
    double coherent_gain; /**< Квадрат суммы коэф. окна, деленной на size */
    double enbw; /**< Equivalent Noise Bandwidth = inherent_gain/coherent_gain */
    int freq_span; /**< Ширина пика одной частоты в спектре */
    char reserv[36]; /**< Резерв */
} t_lmath_win_params;

/** Тип используемого окна */
typedef enum  {
    LMATH_WINTYPE_HANNING  = 1, /**< Hanning */
    LMATH_WINTYPE_BH_4TERM = 7 /**< 4-Term Blackman Harris */
} t_lmath_wintype;


#ifdef __cplusplus
extern "C" {
#endif

/***************************************************************************//**
    Функция применяет временное окно к входному сигналу. Нормирование амплитуды
    выходного сигнала при этом не выполняется.

    @param[in] src          Массив с отсчетами исходного временного сигнала
    @param[in] size         Количество отсчетов во входном массиве
    @param[in] wintype      Тип окна (значение из #t_lmath_wintype)
    @param[in] param        Указатель на структуру с параметрами для расчета
                            окна (резерв)
    @param[out] winParams   Функция записывает выходные характеристики окна
    @param[out] res         Результирующий сигнал. Может указывать на тот же массив,
                            что и src.
    @return                 Код ошибки
    ***************************************************************************/
int lmath_window(const double *src, int size, int wintype, void *param,
                 t_lmath_win_params *win_params, double *res);

/***************************************************************************//**
    Функция применяет временное окно к входному сигналу и выполняет нормирование
    амплитуды выходного сигнала (делит отсчеты на сумму коэффициентов окна)

    @param[in]  src         Массив с отсчетами исходного временного сигнала
    @param[in]  size        Количество отсчетов во входном массиве
    @param[in]  wintype     Тип окна (значение из #t_lmath_wintype)
    @param[in]  param       Указатель на структуру с параметрами для расчета
                              окна (резерв)
    @param[out] win_params  Функция записывает выходные характеристики окна
    @param[out] res         Результирующий сигнал. Может указывать на тот же массив,
                              что и src.
    @return                 Код ошибки
    ***************************************************************************/
int lmath_scaled_window(const double *src, int size, int wintype, void *param,
                        t_lmath_win_params *win_params, double *res);

/***************************************************************************//**
    Рассчет амплитудного и фазового спектра входного сигнала

    @param[in] src      Массив с отсчетами исходного временного сигнала
    @param[in] size     Количество отсчетов во входном массиве
    @param[in] dt       Интервал времени между отсчетами входного сигнала в
                          секундах
    @param[in] flags    Резерв
    @param[out] amp     В этот массив сохраняется рассчитанный амплитудный
                          спектр размером size/2 + 1 элементов. Может быть NULL.
    @param[out] pha     В этот массив сохраняется рассчитанный фазовый
                          спектр размером size/2 + 1 элементов. Может быть NULL.
    @param[out] df      В этой переменной возвращается изменение частоты между
                          соседними отсчетами рассчитанных спектров в Гц.
                          Может быть NULL.
    @return             Код ошибки
    ***************************************************************************/
int lmath_amp_pha_spectrum(const double *src, int size, double dt, int flags,
                           double *amp, double *pha, double *df);

/***************************************************************************//**
    Оценка постоянной и переменной (по RMS) составляющей входного временного
    сигнала. Функция использует окно для сглаживания разрывов на краях сигнала.

    @param[in]  src     Массив с отсчетами исходного временного сигнала
    @param[in]  size    Количество отсчетов во входном массиве
    @param[out] dc      В этой переменной возвращается рассчитанное значение
                            постоянной составляющей. Может быть NULL.
    @param[out] ac      В этой переменной возвращается рассчитанное значение
                            переменной составляющей. Может быть NULL.
    @return             Код ошибки
*******************************************************************************/
int lmath_acdc_estimation(const double *src, int size, double *dc, double *ac);


/***************************************************************************//**
    Поиск значения частоты с максимальной мощностью и мощности сигнала на этой
    частоте по временному сигналу. Функция внутри себя применяет БПФ и анализирует
    спектр. Если уже рассчитан спектр, то стоит использовать
    lmath_find_peak_freq_spectrum()

    @param[in]  src             Массив с отсчетами исходного временного сигнала
    @param[in]  size            Количество отсчетов во входном массиве
    @param[in]  dt              Интервал времени между отсчетами входного
                                  сигнала в секундах
    @param[in]  search_freq     Если значение больше нуля, то ищется частота в
                                  окрестности указанной, иначе - во всей частотной
                                  области (кроме постоянной составляющей)
    @param[out] peak_freq       В этой переменной возвращается рассчитанное значение
                                  частоты, соответствующей максимальной мощности.
                                  Может быть NULL.
    @param[out] peak_pwr        В этой переменной возвращается рассчитанное значение
                                  мощности сигнала на найденной частоте.
                                  Может быть NULL.
    @return                     Код ошибки
*******************************************************************************/
int lmath_find_peak_freq(const double *src, int size, double dt, double search_freq,
                         double *peak_freq, double *peak_pwr);


/***************************************************************************//**
    Поиск значения частоты с максимальной мощностью и мощности сигнала на этой
    частоте по амплитудному спектру.

    @param[in]  amp_spectrum    Амплитудный спектр входного сигнала
    @param[in]  size            Количество отсчетов в спектре
    @param[in]  df              Изменение частоты между соседними отсчетами
                                  спектра в Гц
    @param[in]  search_freq     Если значение больше нуля, то ищется частота в
                                  окрестности указанной, иначе - во всей частотной
                                  области (кроме постоянной составляющей)
    @param[in]  win_params      Характеристики окна, примененного ко входному
                                  сигналу перед вычислением спектра
    @param[out] peak_freq       В этой переменной возвращается рассчитанное значение
                                  частоты, соответствующей максимальной мощности.
                                  Может быть NULL.
    @param[out] peak_pwr        В этой переменной возвращается рассчитанное значение
                                  мощности сигнала на найденной частоте.
                                  Может быть NULL.
    @return                     Код ошибки
*******************************************************************************/
int lmath_find_peak_freq_spectrum(const double *amp_spectrum, int size, double df,
                                  double search_freq, t_lmath_win_params win_params,
                                  double *peak_freq, double *peak_pwr);


/***************************************************************************//**
    Вычисление спектральных параметров по амплитудному спектру сигнала.

    @param[in]  amp_spectrum    Амплитудный спектр входного сигнала
    @param[in]  size            Количество отсчетов в спектре
    @param[in]  df              Изменение частоты между соседними отсчетами
                                  спектра в Гц
    @param[in]  peak_freq       Частота основного сигнала. Должна соответствовать
                                   отсчету с номером не меньше чем freq_span/2.
    @param[in]  harm_cnt        Количество гармоник (включая основную), используемых
                                    при расчете.
    @param[in]  win_params      Характеристики окна, примененного ко входному
                                  сигналу перед вычислением спектра
    @param[out] SNR             Рассчитанное значение отношения сигнал/шум.
                                  Может быть NULL.
    @param[out] THD             Рассчитанное значение Total Harmonic Distortion:
                                  сигнал/гармоники.
                                  Может быть NULL.
    @param[out] SINAD           Рассчитанное значение signal-to-noise-and-distortion
                                  ratio: сигнал/(шум+гармоники).
                                  Может быть NULL.
    @param[out] SFDR            Рассчитанное значение spurious free dynamic range:
                                    макс. сигнал/макс. гармонику вне сигнала.
                                  Может быть NULL.
    @param[out] ENOB            Рассчитанное значение эффективный бит.
                                  Может быть NULL.
    @return                     Код ошибки
*******************************************************************************/
int lmath_calc_spectrum_params (const double *amp_spectrum, int size,  double df,
                                double peak_freq, int harm_cnt, t_lmath_win_params win_params,
                                double *SNR, double *THD, double *SINAD,
                                double *SFDR, double *ENOB);

/***************************************************************************//**
    Вычисление следующего отсчета на выходе FIR-фильтра

    @param[in]  coef            Набор коэффициентов (массив из tap-чисел)
    @param[in, out]  prev_x     Набор предыдущих значений, подаваемых на вход
                                фильтра. Массив из tap-чисел (первый элемент ---
                                самый новый).
                                Для расчета первого значения как правило заполняется
                                нулями. Функция обновляет этот массив и для расчета
                                последующих значений передается массив, полученный
                                при вычислении предыдущего значения.
                                Если используются параллельно несколько фильтров,
                                то для каждого должен использоваться свой
                                массив текущих рассчитанных значений.
    @param[in] ntap             Количество коэффициентов фильтра
    @param[in] new_x            Новое значение на входе фильтра
    @return                     Новое рассчитанное значение на выходе фильтра
*******************************************************************************/
double lmath_proc_fir(const double *coef, double *prev_y, int ntap, double new_x);

#ifdef __cplusplus
}
#endif

#endif // LTEST_MATH_H