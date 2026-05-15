// Compila o header ei_run_classifier.h em um TU isolado.
//
// run_classifier() e definido inline (extern "C") em ei_run_classifier.h, e
// model_variables.h (puxado transitivamente) materializa impulse_handle_996605_1
// com constructor que chama ei_malloc. Mantendo o include APENAS neste arquivo
// (e fora de infer_main.cpp), evitamos puxar esses globais para o TU principal.
//
// Padrao identico ao usado pelos arquivos em `edge-impulse/inference/` do
// repositorio de exemplo edgeimpluse-runner.

#include "edge-impulse-sdk/classifier/ei_run_classifier.h"
