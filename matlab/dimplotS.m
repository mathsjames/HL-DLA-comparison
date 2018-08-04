avglen=40;
fileID=fopen('SierDim');
A=fread(fileID,'double');
fclose(fileID);
A=sort(A);
gap=1/(2*length(A));
LA=A;
LG=[gap:(2*gap):1];
for i=1:length(A)
    LA(i)=log(A(i));
    LG(i)=log(LG(i));
end
SR(k)=2;
for i=1:(length(A)-avglen+1)*0.8
    sierratios(i)=(LG(i+avglen-1)-LG(i))/(LA(i+avglen-1)-LA(i));
    SR(k)=min(SR(k),sierratios(i));
end

figure
plot(sierratios)
