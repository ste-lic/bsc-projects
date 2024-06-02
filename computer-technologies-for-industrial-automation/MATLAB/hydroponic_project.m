clear, close all, clc 
run parametri_prj;
%
% Il nostro sistema può essere modellato come: 
% C_gr * (dT/dt) = q - k_con(T - T_b)
%
%
% T è la nostra variabile da controllare e rappresenta la temperatura 
% all'interno della gemrination room
%

% con questo comando definisco la variabile di Laplace
s = tf('s');         

A = -k_con/C_gr;
B = [1/C_gr k_con/C_gr];
C = 1;
D = [0 0];


% autovalori matrice dinamica
disp("Autovalori della matrice dinamica A: ");
eig(A)               

% funzione di trasferimento del processo
disp("F.d.T del processo: ");
G = tf(ss(A,B,C,D))  
G = G(1)  % Scelgo la prima funzione con primo input 


% plot per margini 
%figure(2)
%subplot(2,2,1)
%margin(G), grid


%% Sintesi controllore con formule di inversione
wg = 1;           % pulsazione di attraversamento
PM = 70;          % margine di fase

% Calcolo modulo e fase del sistema a ciclo aperto a w=wg
[M, P] = bode(G, wg)

Mg = wg/M;
phig = (PM-90-P)*pi/180;

% Formule di inversione
disp("Regolatore sintetizzato mediante formule di inversione: ");
Kp_inv = Mg*sin(phig)/wg;
Ti_inv = tan(phig)/wg;
Ki_inv = Kp_inv/Ti_inv;
 
% otteniamo esattamente un PI
C_inv = Kp_inv+Ki_inv/s
 
% FdT ad anello aperto
disp("Calcolo FdT anello aperto");
L_inv = C_inv*G     

% plot per margini FdT ad anello aperto
figure(1)
subplot(2,2,1)
margin(L_inv), grid

% plot risposta a gradino 
figure(2)
subplot(2,2,3:4)
step(L_inv), grid, title("Risposta a gradino unitario L(iw)")

% Fdt ad anello a ciclo chiuso 
disp("Calcolo FdT anello chiuso");
F = feedback(L_inv,1)

% plot risposta a gradino 
figure(1)
subplot(2,2,3:4)
step(F), grid, title("Risposta a gradino unitario F(iw)")

% plot per margini FdT ad anello chiuso
figure(1)
subplot(2,2,2)
margin(F), grid

% Display info risposta a gradino
Z = stepinfo(F)






