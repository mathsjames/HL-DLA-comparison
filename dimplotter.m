function [R,ratios] = dimplotter (filelist,numtrials,avglen)

clustercount=size(filelist)(1);
ratios=zeros(clustercount,floor((numtrials-avglen+1)*0.8));
R=zeros(clustercount,1);
for k=1:clustercount
printf(strcat('loading :',filelist(k,:),"\n"));
fileID=fopen(strcat(filelist(k,:),''));
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
R(k)=2;
for i=1:(length(A)-avglen+1)*0.8
    ratios(k,i)=(LG(i+avglen-1)-LG(i))/(LA(i+avglen-1)-LA(i));
    R(k)=min(R(k),ratios(k,i));
end
end
figure
plot(transpose(ratios))
figure
hist(R)
maxR=max(R)
minR=min(R)
meanR=mean(R)
sdR=sqrt(var(R))
#B=harmmean(ratios);
#figure
#plot(B)
#minB=min(B)

endfunction
