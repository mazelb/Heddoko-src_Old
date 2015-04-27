%
% Heddoko, Apr 27, 2015
%
% Determining the relationship between forearm rotation
% and StretchSense capacitance.
%


% Get the thresholds for our sample.
dataMin = min(data);
dataMax = max(data);
range = dataMax - dataMin;

% Loop through data and calculate forearm orientation.
dataLength = length(data);
avg = zeros(dataLength, 1);
angle = zeros(dataLength, 1);
for i = 1:dataLength
    if i > 5
        avg(i) = (data(i) + avg(i - 1) + avg(i - 2) + avg(i - 3) + avg(i - 4) + avg(i - 5)) / 6;
    else
        avg(i) = data(i);
    end
    
    % Calculate forearm orientation.
    angle(i) = (dataMax - avg(i))/range * 160;
end

% Plot raw data.
figure
plot( time,data, time,avg )
xlabel('Time (s)')
ylabel('Capacitance')
title('Sensor data')
legend('show')
legend('Raw data', 'Averaged data', 'Location', 'northoutside');

% Plot angle data.
figure
plot( time,angle )
xlabel('Time (s)')
ylabel('Forearm orientation (degrees)')
title('Orientation');
