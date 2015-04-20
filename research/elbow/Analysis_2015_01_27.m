%
% Heddoko, Jan 27, 2015
%
% Elbow data analysis
% (See Google Drive / Engineering / Sensors Research /
%       Test procedures and results / 2015-01-26_Elbow)
%


% Initializing variables
dataLength = length(data);
rate = zeros(dataLength, 1);
rate2 = zeros(dataLength, 1);
data2 = zeros(dataLength, 1);

% Loop through data
for i = 1:dataLength
    % Filtering data
    if i > 5
        data2(i) = (data(i) + data2(i - 1) + data2(i - 2) + data2(i - 3) + data2(i - 4) + data2(i - 5)) / 6;
    else
        data2(i) = data(i);
    end
    
    % Calculate the rate of change
    if i > 1
        rate(i) = (data2(i) - data2(i - 1)) / (time(i) - time(i - 1));
    else
        rate(i) = 1;
    end
    
    % Filtering rate of change
    if i > 5
        rate2(i) = (rate(i) + rate2(i - 1) + rate2(i - 2) + rate2(i - 3) + rate2(i - 4) + rate2(i - 5)) / 6;
    else
        rate2(i) = rate(i);
    end
end

% Plot raw data
figure
plot( time,data, time,data2 )
xlabel('Time')
ylabel('Data points')
title('Sensor data')
legend('show')
legend('Raw data', 'Averaged data')

% Plot raw data
%figure
%plot( time,data )
%title('Raw data')

% Plot filtered data
figure
plot( time,data2 )
title('Averaged data')

% Plot rate of change
%figure
%plot( time,rate )
%title('Rate of change')

% Plot filtered rate of change
figure
plot( time,rate2 )
title('Rate of change')


