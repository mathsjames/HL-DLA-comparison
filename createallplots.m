H00filelist=[];
Bfilelist=[];
for k=1:100
 H00filelist=char(H00filelist,strcat('DimH/dimN100000S',strcat(num2str(k),'SIG0.00A0.00')));
 Bfilelist=char(Bfilelist,strcat('DimB/Dim',num2str(k)));
endfor
Bfilelist=Bfilelist(2:101,:);
H00filelist=H00filelist(2:101,:);
[H00R,H00ratios]=dimplotter(H00filelist,100000,400);
printf("All plots created\n")
