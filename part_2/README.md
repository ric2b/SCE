#SCE Parte II

##Funcionalidades a implementar

⋅⋅*Comunicação série RS232 para permitir o acesso remoto.
⋅⋅*Realização das operações de reconfiguração e transferência da informação recolhida.
⋅⋅*Notificação assíncrona de memória cheia (registos não transferidos > NREG / 2) através da linha série RS232.
⋅⋅*Acrescentar os eventos EMPM e ENMCH relativos à modificação do período de monitorização e à notificação de memória cheia.

###Eventos a adicionar

| evento 		| código		| parâmetros						| descrição															|
|---------------|:-------------:|:---------------------------------:|:-----------------------------------------------------------------:|
| EMPM			| 10			| antigo valor (1) e novo valor(1)	| Colocar em memória quando se modifica o periodo de modificação 	|
| ENMCH			| 11			| NREG(1) + nr(1) + ie(1) +il(1)	| Notificação memória cheia										 	|

###Interface com o utilizador

Exitem três blocos de comandos:
⋅⋅*comandos que interagem com a tarefa de comunicação
⋅⋅*big description no pdf
⋅⋅*comandos que interagem com a tarefa de processamento

###Possivel ordem de implementação

1. Leitura do terminal
2. Criação das threads e manutenção dos metódos de comunicação entre threads
3. Comunicação série RS232 (Isto talvez tenha que passar para o inicio)a
4. Acesso a variaveis da placa (AKA apenas os comandos que pedem para mostrar valores)
5. Restantes operações 