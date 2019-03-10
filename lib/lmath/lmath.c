#include "lmath.h"

#include <math.h>
#include <fftw3.h>
#include <stdlib.h>
#include <string.h>

#ifndef M_PI
    #define M_PI 3.1415926535
#endif

#ifndef MAX
    #define MAX(x,y) ((x) > (y) ? (x) : (y))
#endif

typedef double (*t_win_pont_func)(int i, int n, void* param);

static double f_bh4_win_point(int i, int n, void *param);
static double f_hanning_win_point(int i, int n, void *param);

static t_win_pont_func f_win_func_tbl[] = {
    0,
    f_hanning_win_point,
    0,
    0,
    0,
    0,
    0,
    f_bh4_win_point
};

static double f_bh4_win_point(int i, int n, void *param) {
    const double a0 = 0.35875;
    const double a1 = 0.48829;
    const double a2 = 0.14128;
    const double a3 = 0.01168;
    double a= (2.*M_PI*i)/n;
    return a0 - a1*cos(a) + a2*cos(2*a) - a3*cos(3*a);
}

static double f_hanning_win_point(int i, int n, void* param) {
    double a= (2.*M_PI*i)/n;
    return 0.5*(1-cos(a));
}

int lmath_window(const double *src, int size, int wintype, void *param,
                 t_lmath_win_params *win_params, double *res) {

    int err = 0;

    if ((wintype >= sizeof(f_win_func_tbl)/sizeof(f_win_func_tbl[0])) ||
            (f_win_func_tbl[wintype]==0)) {
        err = LMATH_ERR_INVALID_WINTYPE;
    } else {
        double sum =0.0;
        double sum2 = 0;
        int i;
        t_win_pont_func func = f_win_func_tbl[wintype];
        for (i=0; i < size; i++) {
            double val = func(i, size, param);
            sum+=val;
            sum2 += (val*val);
            res[i] = src[i]*val;
        }

        if (win_params != NULL) {
            memset(win_params, 0, sizeof(t_lmath_win_params));
            win_params->coherent_gain = pow(sum/size,2);
            win_params->inherent_gain = sum2/size;
            win_params->enbw = win_params->inherent_gain/win_params->coherent_gain;
            win_params->freq_span = 9;
        }
    }

    return err;
}

int lmath_scaled_window(const double *src, int size, int wintype, void *param,
                        t_lmath_win_params *win_params, double* res) {
    int err = lmath_window(src, size, wintype, param, win_params, res);
    if (!err) {
        int i;
        double del = sqrt(win_params->coherent_gain);
        for (i=0; i < size; i++)
            res[i] /= del;        
    }
    return err;
}



int lmath_amp_pha_spectrum(const double *src, int size, double dt, int flags,
                           double *amp, double *pha, double *df) {
    int fft_size = size/2 + 1;
    int i;
    int err = 0;

    fftw_complex* fft = (fftw_complex*)fftw_malloc(sizeof(fftw_complex)*fft_size);
    double *fsig = (double*)fftw_malloc(sizeof(double)*size*2);
    fftw_plan p = fftw_plan_dft_r2c_1d(size, fsig, fft, FFTW_ESTIMATE);

    if ((fft != NULL) && (fsig != NULL) && (p != NULL)) {
        memcpy(fsig, src, sizeof(double)*size);
        fftw_execute(p);

        for (i=0; i < fft_size; i++) {
            if (amp != NULL)
                amp[i] = sqrt((fft[i][0]*fft[i][0] + fft[i][1]*fft[i][1])/2)*(i?2:1)/size;
            if (pha != NULL)
                pha[i] = atan2(fft[i][1]*fft[i][1], fft[i][0]*fft[i][0]);
        }

        if (df != NULL)
            *df = 1./(dt*size);

    } else {
        err = LMATH_ERR_MEMORY_ALLOC;
    }

    fftw_destroy_plan(p);
    fftw_free(fft);
    fftw_free(fsig);

    return err;
}






int lmath_calc_spectrum_params (const double *ampSpectrum, int size,  double df,
                                double peak_freq, int harm_cnt, t_lmath_win_params win_params,
                                double *SNR, double *THD, double *SINAD, double *SFDR, double *ENOB) {
    int res=0;
    int i;
    int ipeak = (int)(peak_freq/df + 0.5);
    int freq_span = win_params.freq_span;
    double PeakPower=0;
    double NoisePower=0;
    double HarmPower=0;
    double HarmNoisePower =0;
    double MaxPower=-100000000;
    double MaxSygnal=-100000000;

    if (SNR!=NULL)
        *SNR=0;
    if (THD!=NULL)
        *THD=0;
    if (SINAD!=NULL)
        *SINAD=0;
    if (SFDR!=NULL)
        *SFDR =0;
    if (ENOB!=NULL)
        *ENOB=0;

    if (ipeak < (freq_span / 2))
        return LMATH_ERR_INVALID_PARAMS;

    for(i=freq_span/2; i<size;i++) {
        int NearestPeak = (i+(freq_span / 2))/ipeak;
        int NearestMaxPeak = (NearestPeak*ipeak)+(freq_span / 2);
        int NearestMinPeak = (NearestPeak*ipeak)-(freq_span / 2);

        double RMS = ampSpectrum[i];

        // используется только первые harm_cnt гармоник
        if ((i>=NearestMinPeak) && (i<=NearestMaxPeak) &&
                (NearestPeak>0) && (NearestPeak<harm_cnt)) {
            if(NearestPeak > 1) {
                MaxSygnal = MAX(MaxSygnal, RMS);
                HarmPower += (RMS*RMS);
            } else {
                MaxPower = MAX(MaxPower, RMS);
                PeakPower += (RMS*RMS);
            }
        } else {
            MaxSygnal = MAX(MaxSygnal, RMS);
            NoisePower+=(RMS*RMS);
        }
    }

    PeakPower = PeakPower/win_params.enbw;
    HarmPower = HarmPower/win_params.enbw;
    NoisePower = NoisePower/win_params.enbw;
    HarmNoisePower = NoisePower+HarmPower;

    if(PeakPower != 0) {
        double DynamicRange = MaxPower/MaxSygnal;
        if ((DynamicRange>0) && (SFDR!=NULL)) {
            *SFDR = 20 * log10(DynamicRange);
        }

        if (NoisePower) {
            double SNRPower =  PeakPower / NoisePower;
            if((SNRPower>0) && (SNR!=NULL)) {
                *SNR = 10 * log10(SNRPower);
                if (*SNR < 0) {
                    *SNR = 0;
                    res=LMATH_ERR_INTERNAL_CALC;
                }
            }
        }

        if(HarmPower) {
            double THDPower = HarmPower/PeakPower;
            if ((THDPower>0) && (THD!=NULL)) {
                *THD = 10 * log10(THDPower);
                if (*THD >0) {
                    *THD = 0;
                    res=LMATH_ERR_INTERNAL_CALC;
                }
            }
        }

        if (HarmNoisePower) {
            double SINADPower = PeakPower/HarmNoisePower;
            if ((SINADPower>0) && (SINAD!=NULL)) {
                double sinad = 10*log10(SINADPower);
                if (sinad<0) {
                    res=LMATH_ERR_INTERNAL_CALC;
                } else {
                    if (SINAD!=NULL) {
                        *SINAD=sinad;
                    }

                    if (ENOB!=NULL) {
                        *ENOB = (sinad - 1.76) / 6.02;
                    }
                }
            }
        }
    }
    return res;
}




int lmath_acdc_estimation(const double *src, int size, double *dc, double *ac) {
    int err = 0;
    double *tmp_arr = malloc(sizeof(tmp_arr[0])*size);
    t_lmath_win_params win_par;
    double avg, rms;
    int i;

    if (tmp_arr==NULL) {
        err = LMATH_ERR_MEMORY_ALLOC;
    } else {
        /* сперва накладываем окно на исходный сигнал для рассчета DC */
        err = lmath_window(src, size, LMATH_WINTYPE_BH_4TERM, 0, &win_par, tmp_arr);
    }

    if (!err) {
        for (avg=0, i=0; i < size; i++) {
            avg+=tmp_arr[i];
        }
        avg/=(size*sqrt(win_par.coherent_gain));
    }

    if (!err) {
        /* далее для рассчета AC заменяем сигнал на исходный без DC */
        for (i=0; i < size; i++) {
            tmp_arr[i]=(src[i] - avg);
        }

        /* накладываем окно на сигнал без DC */
        err = lmath_window(tmp_arr, size, LMATH_WINTYPE_BH_4TERM, 0, &win_par, tmp_arr);
        for (rms=0, i=0; i < size; i++) {
            rms+= tmp_arr[i]*tmp_arr[i];
        }

        rms/=(size*win_par.inherent_gain);
        rms = sqrt(rms);
    }

    free(tmp_arr);

    if (dc!=NULL)
        *dc = avg;
    if (ac!=NULL)
        *ac = rms;
    return err;
}


int lmath_find_peak_freq(const double *src, int size, double dt, double search_freq,
                         double *peak_freq, double *peak_pwr) {
    int err = 0;
    double *proc_vals = malloc(sizeof(proc_vals[0])*size);
    t_lmath_win_params win_par;
    double df;


    if (proc_vals==NULL) {
        err = LMATH_ERR_MEMORY_ALLOC;
    } else {
        /* амплитуду берем только на заданной частоте по спектру */
        err = lmath_scaled_window(src, size, LMATH_WINTYPE_BH_4TERM, 0, &win_par, proc_vals);
    }

    if (!err) {
        err = lmath_amp_pha_spectrum(proc_vals, size, dt, 0, proc_vals, 0, &df);
    }

    if (!err) {
        err = lmath_find_peak_freq_spectrum(proc_vals, size/2+1, df, search_freq,
                                            win_par, peak_freq, peak_pwr);
    }

    free(proc_vals);

    return err;
}





int lmath_find_peak_freq_spectrum(const double *amp_spectrum, int size, double df, double search_freq,
                                  t_lmath_win_params win_params,
                                  double *peak_freq, double *peak_pwr) {
    int i, fnd_i;
    double fnd_pwr = 0;
    int freq_span = win_params.freq_span;

    if ((amp_spectrum==NULL) || ((peak_freq==NULL) && (peak_pwr==0)) ||
            (df <= 0) || (freq_span < 1)) {
        return LMATH_ERR_INVALID_PARAMS;
    }

    /* если не задана частота, в окрестности которой нужно найти требуемую,
     * то ищем во всем спектре по максимальной мощьности (исключая пост. составляющую) */
    if (search_freq <= 0) {
        for (fnd_i=0,i= freq_span/2 + freq_span/2; i < (size - freq_span/2); i++) {
            double pwr=0;
            int j;
            for (j=i-freq_span/2; j <= (i+freq_span/2); j++) {
                pwr+=(amp_spectrum[j]*amp_spectrum[j]);
            }

            if (pwr > fnd_pwr) {
                fnd_pwr = pwr;
                fnd_i = i;
            }
        }
    } else {
        /* иначе рассчитываем мощность только в окрестности указанной частоты */
        int j;
        fnd_i = (int)(search_freq/df + 0.5);
        if (fnd_i >= (size - freq_span/2))
            fnd_i = size - freq_span/2 - 1;
        if (fnd_i < freq_span)
            fnd_i = freq_span;

        for (fnd_pwr=0, j=fnd_i-freq_span/2; j <= (fnd_i+freq_span/2); j++) {
            fnd_pwr+=(amp_spectrum[j]*amp_spectrum[j]);
        }
    }

    if (peak_freq!=NULL) {
        double fnd_freq=0;
        int j;
        /* поиск частоты сигнала */
        for (j= fnd_i-freq_span/2; j <= (fnd_i+freq_span/2); j++) {
            fnd_freq+=(amp_spectrum[j]*amp_spectrum[j])*j*df;
        }
        fnd_freq/=fnd_pwr;

        *peak_freq = fnd_freq;
    }

    if (peak_pwr!=NULL) {
        *peak_pwr = fnd_pwr/win_params.enbw;
    }

    return 0;
}


double lmath_proc_fir(const double *coef, double *prev_x, int ntap, double new_x) {
    double y=0;            //output sample
    int n;

    //shift the old samples
    for(n=ntap-1; n>0; n--)
       prev_x[n] = prev_x[n-1];

    //Calculate the new output
    prev_x[0] = new_x;
    for(n=0; n<ntap; n++)
        y += coef[n] * prev_x[n];

    return y;
}
