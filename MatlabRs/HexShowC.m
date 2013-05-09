% Kolijn Wolfaardt
% May 2013
% 
% Convert Intel Hex format to image
% This script assumes the data stored in the intel hex file is a 100*100
% array, with each byte being a grayscale value. The start addres, and all
% other data is discarded, as well as the checksums.
%
% To export the data from Keil, open the debugger, let the code run, and
% use the debug window to execute:
%
%   save filename startAddr,stopAddr
%
%  For example:
%
%   save MyValues.hex 0x20002710, 0x20004e20

fileName = 'Col.hex';
file_handle  = fopen(fileName);


text = fgets(file_handle);

size = 100;
data = zeros(size,size);
pos  = 1;

while text ~= -1
    %First, get the length of the line
    ll = hex2dec(text(2:3));
    aaaa = hex2dec(text(4:7));
    tt = hex2dec(text(8:9));
    
    dataS = text(10:10+ll*2-1);
    
    %In intel hex, if the typpe is 00 it is data
    %The rest can be safely ignored
    if (tt==0) && (pos<=size^2)
        for k=1:ll
            d = hex2dec(dataS(k*2-1:k*2));
            data(pos) = d;
            pos = pos+1;
        end
    end
    
    text = fgets(file_handle);
end
% imshow() needs values between 0.0 and 1.0
% First, shift the data 128 up, so that positve and negative numbers are
% displayed better. Then divide by 256.
data = mod(data+128,256)/256;

figure();
imshow(data);

fclose(file_handle);
