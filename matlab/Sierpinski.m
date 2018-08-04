Sier=[0+2i/3];
drop=1i/2;
shift=2/sqrt(3);
for n=1:13
Sier=[Sier;Sier-drop+shift;Sier-drop-shift];
drop=drop/2;
shift=shift/2;
end
Folded=zeros(1,length(Sier)*2);
for count=1:length(Sier)
    Folded(2*count-1)=real(Sier(count));
    Folded(2*count)=imag(Sier(count));
end
fileID=fopen('Sierpinski','w');
fwrite(fileID,Folded,'double');
fclose(fileID);
