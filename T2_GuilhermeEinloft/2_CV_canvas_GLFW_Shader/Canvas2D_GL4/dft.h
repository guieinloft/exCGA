/* DFT.h
 * conjunto de funções com implementações da DFT e da CDFT (e suas inversas)
 * além da obtenção das informações (amplitude, frequência e fase) dos sinais
 */

/* aplica a DFT padrão sobre um sinal real (1D) */
void dft(float *x, float *yre, float *yim, int n);
/* aplica a IDFT, gerando um sinal real (1D) */
void idft(float *x, float *yre, float *yim, int n);
/* aplica a CDFT sobre um sinal complexo (2D) */
void cdft(float *xre, float *xim, float *yre, float *yim, int n);
/* aplica a CIDFT, gerando um sinal complexo (2D) */
void cidft(float *xre, float *xim, float *yre, float *yim, int n);
/* obtém informações (frequência, fase e amplitude) de um sinal */
void siginfo(float *yre, float *yim,
		float *yphase, float *yamp, float *yfreq, int n);
/* imprime informações de um sinal */
void print_siginfo(float *yre, float *yim,
		float *yphase, float *yamp, float *yfreq, int n);
/* ordena os epicycles por amplitude */
void sort_amplitude(float *yre, float *yim,
		float *yphase, float *yamp, float *yfreq, int n);
