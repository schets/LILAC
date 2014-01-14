%The RHS of the equation
function rhs=CNLS_fft_rhs(z,U,kt,D,K,A,B,g0,e0,tau,Gamma,dt,t,nt);
u=ifft(U(1:nt,1));
v=ifft(U((nt+1):2*nt,1));
u_s=U(1:nt,1);
v_s=U((nt+1):2*nt,1);
au2=abs(u).^2;
av2=abs(v).^2;
kt2 = kt.^2;
expr1 = 1i*(2*g0/(1+1/e0*trapz(au2+av2)*dt));
expr1
rhs(1:nt,1)=((D/2*kt2+K).*u_s-fft((au2+A*av2).*u)-...
    B*fft(v.^2.*conj(u))+expr1...
    *(u_s-tau*kt2.*u_s)-Gamma*u_s)/1i;
rhs((nt+1):2*nt,1)=((D/2*kt2-K).*v_s-fft((A*au2+av2).*v)-...
    B*fft(u.^2.*conj(v))+expr1...
    *(v_s-tau*kt2.*v_s)-Gamma*v_s)/1i;
