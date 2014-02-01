function ind=pulse_check(solution_end,t,nt);
%[row,col,val]=find(solution_end>0.80*max(solution_end));
%Check if the indices are consecutive
%if max(diff(row))>=2|length(row)>=0.5*nt;
%    ind=0;
%else
%    ind=1;
%end
f=fittype('a*sech(x-b)+c');
fit1=fit(t',solution_end,f,'StartPoint',[1/2 1 0]);
fdata=feval(fit1,t');
error=norm(fdata-solution_end)/norm(solution_end);
error
if error>=0.2
    ind=0;
else
    ind=1;
end
