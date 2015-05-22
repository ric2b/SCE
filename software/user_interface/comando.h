#ifndef COMANDO_H
#define COMANDO_H

#include <stdio.h>
#include <stdlib.h>
#include <cyg/io/io.h>

Cyg_ErrNo err;
cyg_io_handle_t serH;

#define SOM   0xFD  /* inicio mensagem */
#define EOM   0xFE  /* fim mensagem */
#define CRLG  0xC0  /* consultar relogio */
#define ARLG  0xC1  /* acertar relogio */
#define CTEL  0xC2  /* consultar temperatura e luminosidade */
#define CPAR  0xC3  /* consultar parametros */
#define MPMN  0xC4  /* modificar periodo monitorizacao */
#define CALA  0xC5  /* consultar alarmes */
#define DALR  0xC6  /* definir alarme relogio */
#define DALT  0xC7  /* definir alarme temperatura */
#define DALL  0xC8  /* definir alarme luminosidade */
#define AALA  0xC9  /* activar/desactivar alarmes */
#define IREG  0xCA  /* informacao sobre registos */
#define TRGC  0xCB  /* transferir registos a partir da posicao corrente */
#define TRGI  0xCC  /* transferir registos a partir do indice especificado */
#define NMCH  0xCD  /* notificacao memoria cheia */
#define CMD_OK    0            /* comando realizado com sucesso */
#define CMD_ERRO  0xFF         /* erro no comando */

#define COMMUNICATION_THREAD 1
#define PROCESSING_THREAD 0

void cmd_cr (int argc, char **argv);
void cmd_ar (int argc, char **argv);
void cmd_ctl (int argc, char **argv);
void cmd_cp (int argc, char **argv);
void cmd_mpm (int argc, char **argv);
void cmd_ca (int argc, char **argv);
void cmd_dar (int argc, char **argv);
void cmd_dat (int argc, char **argv);
void cmd_dal (int argc, char **argv);
void cmd_aa (int argc, char **argv);
void cmd_ir (int argc, char **argv);
void cmd_trc (int argc, char **argv);
void cmd_tri (int argc, char **argv);
void cmd_irl (int argc, char **argv);
void cmd_lr (int argc, char **argv);
void cmd_er (int argc, char **argv);
void cmd_cpt (int argc, char **argv);
void cmd_mpt (int argc, char **argv);
void cmd_lar (int argc, char **argv);
void cmd_lat (int argc, char **argv);
void cmd_lal (int argc, char **argv);
void cmd_iga (int argc, char **argv);
void cmd_ig (int argc, char **argv);
void cmd_sair (int argc, char **argv);
void cmd_test (int argc, char** argv);
void cmd_ems (int argc, char **argv);
void cmd_emh (int argc, char **argv);
void cmd_rms (int argc, char **argv);
void cmd_rmh (int argc, char **argv);
void cmd_ini(int argc, char **argv);

#endif
