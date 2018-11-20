for k=1:10
fileID=fopen(strcat('DimH/dimP2N100000S',int2str(k)));
A(:,k)=sort(fread(fileID,'double'));
fclose(fileID);
fileID=fopen(strcat('DimB/Dim',int2str(k)));
B(:,k)=sort(fread(fileID,'double'));
fclose(fileID);
fileID=fopen(strcat('DimH/dimExactN100000S',int2str(k)));
C(:,k)=sort(fread(fileID,'double'));
fclose(fileID);
end
figure
plot(transpose([0.00001:0.00001:1]),A,'r')
hold on
plot(transpose([0.00001:0.00001:1]),B,'b')
plot(transpose([0.00001:0.00001:1]),C,'g')
hold off