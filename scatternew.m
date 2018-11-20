for k=1:1000
  leng=0;
  d=0;
  while (leng<sqrt(2))
    d++;
    leng=leng+A(d,k)/2500;
  endwhile
  samples(k)=d;
  D(k)=d/2500;
endfor

sig=1000;
for clus=1:100
  prod=1;
  for k=1+(clus-1)*10:clus*10
    c(k)=A(sig,k);
    b(k)=A(1,k);
    prod*=c(k);
  endfor
  prod=prod^0.1;
  for k=1+(clus-1)*10:clus*10
    c(k)=prod;
    b(k)=b(k);
  endfor
endfor
figure, scatter(log(b),c,10,D)
maxsamp=max(samples);
lines=0;
for k=1:1000
  if samples(k)>0
    lines++;
  endif
endfor
X=ones(maxsamp,lines);
Y=ones(maxsamp,lines);
Y=Y*sqrt(2)*2500;
line=0;
for k=1:100
  if samples(k)>0
    line++;
    for sample=1:samples(k)+1
      X(sample,line)=(sample-1);%/samples(k);
      if (sample==1)
        Y(sample,line)=0;
      else
        Y(sample,line)=Y(sample-1,line)+A(sample-1,k);
      endif
    endfor
  endif
endfor
line
%figure, plot(X,Y);
halfsize=zeros(1,lines);
for line=1:lines
  sample=1;
  while X(sample,line)<0.5
    sample++;
  endwhile
  halfsize(line)=Y(sample,line);
endfor

