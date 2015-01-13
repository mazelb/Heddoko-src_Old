%
% Heddoko, Dec 16, 2014
%
% Shoulder motion empirical analysis using stretch sensor data.
%
% Import data into Matlab and rename the variables:
%   rawTop:     Top sensor data
%   rawBack:    Back sensor data
%   rawBody:    Body sensor data
%   time:       Time data
%


% Calculate thresholds for each sensor
thTop = [min(rawTop), max(rawTop)];
thBack = [min(rawBack), max(rawBack)];
thBody = [min(rawBody), max(rawBody)];

% Prepare data
dataLength = min([length(rawTop), length(rawBack), length(rawBody)]);
posForward = zeros(dataLength,1);
posUpward = zeros(dataLength,1);

% Loop through data, format data into values we can use
for i = 1:dataLength
    % Map values to 0-1000
    top = (rawTop(i) - thTop(1)) / (thTop(2) - thTop(1)) * 1000;
    back = (rawBack(i) - thBack(1)) / (thBack(2) - thBack(1)) * 1000;
    body = (rawBody(i) - thBody(1)) / (thBody(2) - thBody(1)) * 1000;
    
    % Determine forward position of arm
    posForward(i) = back;
    
    % Determine upward position of arm
    if body > 100
        posUpward(i) = ((body - 100) + 1000) / 2;
    elseif top > 100
        posUpward(i) = (1000 - (top + 100)) / 2;
    else
        posUpward(i) = 500;
    end
end

% Plot result
figure
plot( time,posForward, time,posUpward )


% ---------------------------------------------------------------------- %
%
% Upward position_Rev 0
%{
    if body > 100
        posUpward(i) = ((body - 100) + 1000) / 2;
    elseif top > 100
        posUpward(i) = (1000 - (top + 100)) / 2;
    else
        posUpward(i) = 500;
    end
}%


