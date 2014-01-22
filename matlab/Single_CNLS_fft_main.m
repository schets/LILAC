clear all
close all
clc
% fftw('planner','measure')
time=tic;
D=-0.4;
K=0.1;
A=2/3;
B=1/3;
tau=0.1;
Gamma=0.1;
mode=0;% if mode=1, load parameter values from CNLS_GA_results.mat
       % else, use default values.
RTlength=15;% roundtrip length
if mode==1
    load CNLS_GA_results.mat
    g01=param(5);
    e01=param(6);
    alpha_1=param(1);
    alpha_2=param(2);
    alpha_3=param(3);
    alpha_p=param(4);
else
    %default parameter values
    %g01=2.03;
    %g01=1.73;
    g01=1.73;
    %e01=4.23;
    e01=4.23;
    %alpha_1=-0.3755;
    alpha_1=-0.4668;
    alpha_2=0.0496;
    alpha_3=0.4860;
    alpha_p=1.5410;
    %alpha_1=0.0;
    %alpha_2=0.3;
    %alpha_2=0.49*pi;
    %alpha_3=0.2*pi;
    %alpha_3=-1.033;
    %alpha_p=0.45*pi;
    alpha = [alpha_1; alpha_2; alpha_3; alpha_p];
end
Lt=60;  % length of t-domain
nt=6; % number of time points
t2=linspace(-Lt/2,Lt/2,nt+1); 
t=t2(1:nt);
dt=Lt/nt;
%Initial Condition
%load IC.mat;
%U_int = ICdata;
U_int(1:nt,1)=sech(t/2);
U_int(nt+1:2*nt,1)=sech(t/2);
% Spectral K Values 
kt2=(2*pi/Lt)*nt/2*linspace(-1,1,nt+1);
kt=fftshift(kt2(1:nt)).';
kt/((2*pi/Lt)*nt/2);
test=linspace(-1,1,nt+1);
test(1:nt);
fftshift(test(1:nt));
%Jones Matrices
W_q=[exp(-1i*pi/4) 0; 0 exp(1i*pi/4)];
W_h=[-1i 0; 0 1i];
W_p=[1 0; 0 0];
J_1=R_matrix(alpha_1)*W_q*R_matrix(-alpha_1);
J_2=R_matrix(alpha_2)*W_q*R_matrix(-alpha_2);
J_3=R_matrix(alpha_3)*W_h*R_matrix(-alpha_3);
J_p=R_matrix(alpha_p)*W_p*R_matrix(-alpha_p);
time=tic;
U_int_0_s(1:nt,1)=fft(U_int(1:nt));
U_int_0_s((nt+1):(2*nt),1)=fft(U_int((nt+1):(2*nt)));
U_int_0_s;
maxTrips = 50;
change_norm=1.e+1000;
norms=[];
j = 1;
while (j<=maxTrips && change_norm>1.e-3)
% for j=1:RoundTrips
    %j
    if j==1
        U_int_s=U_int_0_s;
    else
        U_int_s=[fft(U_solution(1:nt,j-1));fft(U_solution((nt+1):2*nt,j-1))];
    end
    % integrating FFT'd ODE for NLSE
    options = odeset('RelTol',1e-5,'AbsTol',1e-3*ones(nt*2,1));
    % options=[];
%    [RTlength*(j-1):0.01:RTlength*j]
    [z,U]=ode45(@(z,U) CNLS_fft_rhs(z,U,kt,D,K,A,B,g01,e01,tau,Gamma,dt,t,nt),[0, RTlength],U_int_s,options);
   % U
    % iFFT to back out u, v at spatial end of round trip
    u_end=ifft(U(end,1:nt))'
    v_end=ifft(U(end,(nt+1):(2*nt)))'
    solution_end=[u_end, v_end];
    %Apply Jones Matrix
    U_end=J_1*J_p*J_2*J_3*[u_end;v_end];
    % solution containing both small u and v
    U_solution(:,j)=[U_end(1,:).'; U_end(2,:).'];
    U_solution_output(:,j)=U_solution(:,j);
    if(j~=1)
        phi=sqrt(abs(U_solution_output(1:nt,:)).^2+abs(U_solution_output((nt+1):2*nt,:)).^2);
        change_norm=norm((phi(:,end)-phi(:,end-1)))/norm(phi(:,end-1));
        norms=[norms; change_norm];
   end
   break;
   j = j+1;
end
RoundTrips=j-2;
cpu_time=toc(time)
% ICdata = U_solution(:,end);
% save IC.mat ICdata;
% % go through one more loop
% U_int_s=[fft(U_solution(1:nt,j));fft(U_solution((nt+1):2*nt,j))];
% % 
%     [z,U]=ode45(@(z,U) CNLS_fft_rhs(z,U,kt,D,K,A,B,g01,e01,tau,Gamma,dt,t,nt),[RTlength*(j):0.01:RTlength*(j+1)],U_int_s,options);
% u_end=ifft(U(end,1:nt));
% v_end=ifft(U(end,(nt+1):(2*nt)));
% %Apply Jones Matrix
% U_end=J_1*J_p*J_2*J_3*[u_end;v_end];
% U_solution(:,j+1)=[U_end(1,:).'; U_end(2,:).'];
% U_solution_output(:,j+1)=U_solution(:,j+1);
% cpu_time=toc(time)
% phi=sqrt(abs(U_solution_output(1:nt,:)).^2+abs(U_solution_output((nt+1):2*nt,:)).^2);
% change_norm=norm((phi(:,end)-phi(:,end-1)))/norm(phi(:,end-1));
% change_norm
solution_end=phi(:,end);
% compute energy of the pulse
energy=sqrt(trapz(solution_end.^2)*dt);
energy
% function used to check whether the output is a single pulse
%pulse_check(solution_end,t,nt)
% colormap used in 'waterfall'
map=[0 0 0];
figure
plot(t,phi(:,end))
single_sol=phi(:,end);
figure
plot(abs(fftshift(fft(phi(:,end)))))
% compute moments of the spectrum
spec_moment_4=moment(abs(fftshift(fft(phi(:,end)))),4)/std(abs(fftshift(fft(phi(:,end)))))^4
spec_moment_4_2=kurtosis(abs(fftshift(fft(phi(:,end)))))
spec_moment_5=moment(abs(fftshift(fft(phi(:,end)))),5)/std(abs(fftshift(fft(phi(:,end)))))^5
spec_moment_6=moment(abs(fftshift(fft(phi(:,end)))),6)/std(abs(fftshift(fft(phi(:,end)))))^6
single_phi=phi;
save single_sol_512.mat single_sol single_phi
