fileID=fopen('SolaCode/bin/locationS2');
rawparticles=fread(fileID,'double');
fclose(fileID);
particles=zeros(1,length(rawparticles)/2);
for count=1:length(rawparticles)/2
particles(count)=rawparticles(2*count-1)+1i*rawparticles(2*count);
end
drawcluster
