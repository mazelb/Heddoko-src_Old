%
% Heddoko, Jan 15, 2015
%
% Shoulder motion empirical analysis using stretch sensor data.
%
% Import data into Matlab (making sure to import as integers, not text)
% and rename the variables:
%   rawTop:     Top sensor data
%   rawBack:    Back sensor data
%   rawFront:   Front sensor data
%   time:       Time data
%


% Calculate thresholds for each sensor
thTop = [min(rawTop), max(rawTop)];
thBack = [min(rawBack), max(rawBack)];
thFront = [min(rawFront), max(rawFront)];

% Prepare data
dataLength = min([length(rawTop), length(rawBack), length(rawFront)]);
posForward = zeros(dataLength,1);
posUpward = zeros(dataLength,1);

% Loop through data, format data into values we can use
for i = 1:dataLength
    % Map values to 0-1000
    top = (rawTop(i) - thTop(1)) / (thTop(2) - thTop(1)) * 1000;
    back = (rawBack(i) - thBack(1)) / (thBack(2) - thBack(1)) * 1000;
    front = (rawFront(i) - thFront(1)) / (thFront(2) - thFront(1)) * 1000;
    
    % Determine forward position of shoulder
    posForward(i) = (back - front + 1000) / 2;
    
    % Determine upward position of arm
	posUpward(i) = top;
end

% Plot raw data
figure
plot( time,rawTop, time,rawBack, time,rawFront );
legend('show')
legend('Top sensor', 'Back sensor', 'Front sensor')

% Plot result
figure
plot( time,posForward, time,posUpward );
