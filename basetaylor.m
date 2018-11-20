B=zeros(1000);
n=3;
for k=1:1000
  p=polyfit(transpose([1:100]),A(1:100,k),n);
  B(:,k)=polyval(p,transpose([1:1000]));
end

for k=1:1000
  leng=0;
  d=0;
  while (leng<sqrt(2) && d<1000)
    d++;
    leng=leng+B(d,k)/2500;
  endwhile
  samplesB(k)=d;
  DB(k)=d/2500;
endfor
figure, scatter(samples,samplesB)
