
for k=1:10
fileID=fopen(strcat('asizes/asizeP2SIG0.000L1.414A2.00N100000S',int2str(k)));
AS(:,k)=fread(fileID,'double');
fclose(fileID);
fileID=fopen(strcat('sizes/sizeP2SIG0.000L1.414A2.00N100000S',int2str(k)));
S(:,k)=fread(fileID,'double');
fclose(fileID);
end

corr(S(:),AS(:))
