%
% Heddoko, Apr 27, 2015
%
% Testing equations for elbow bending and forearm rotation.
%
% Sample data:
%   data/Data_2015-02-26/xlsx
%   data/2015-04-23_Military push ups.xlsx
%   data/2015-04-23_Triceps push ups.xlsx
%   data/2015-04-23_Wide push ups.xlsx

%%
% 1: Configuration.
%

% Whether to use the raw data or an average.
useAveragedData = true;

%%
% 2: Define thresholds for our data sample.
%

% Set thresholds for elbow bending.
%ssRightElbowExtMin = 1200;
ssRightElbowExtMin = min(ssRightElbowExt);
%ssRightElbowExtMax = 1500;
ssRightElbowExtMax = max(ssRightElbowExt);
ssRightElbowExtRange = ssRightElbowExtMax - ssRightElbowExtMin;

% Set thresholds for forearm rotation.
%ssRightForearmSupMin = 1200;
ssRightForearmSupMin = min(ssRightForearmSup);
%ssRightForearmSupMax = 1350;
ssRightForearmSupMax = max(ssRightForearmSup);
ssRightForearmSupRange = ssRightForearmSupMax - ssRightForearmSupMin;

%%
% 3: Apply mapping equations to sample data.
%

% Prepare data points.
dataLength = min([length(ssRightElbowExt), length(ssRightForearmSup)]);
rightElbowExt = zeros(dataLength, 1);
rightForearmSup = zeros(dataLength, 1);

% Define variables to store averaged data.
ssRightElbowExtAvg = zeros(dataLength, 1);
ssRightForearmSupAvg = zeros(dataLength, 1);

% Loop through data
for i = 1:dataLength
    
    % Average the data and make it smoother.
    if i > 3
        ssRightElbowExtAvg(i) = (ssRightElbowExt(i) + ssRightElbowExtAvg(i - 1) + ssRightElbowExtAvg(i - 2) + ssRightElbowExtAvg(i - 3)) / 4;
        ssRightForearmSupAvg(i) = (ssRightForearmSup(i) + ssRightForearmSupAvg(i - 1) + ssRightForearmSupAvg(i - 2) + ssRightForearmSupAvg(i - 3)) / 4;
    else
        ssRightElbowExtAvg(i) = ssRightElbowExt(i);
        ssRightForearmSupAvg(i) = ssRightForearmSup(i);
    end
    
    % Calculate angle inside elbow.
    if (useAveragedData)
        rightElbowExt(i) = (ssRightElbowExtMax - ssRightElbowExtAvg(i))/ssRightElbowExtRange * (180 - 40) + 40;
    else
        rightElbowExt(i) = (ssRightElbowExtMax - ssRightElbowExt(i))/ssRightElbowExtRange * (180 - 40) + 40;
    end
    
    % Calculate forearm orientation.
    if (useAveragedData)
        rightForearmSup(i) = (ssRightForearmSupMax - ssRightForearmSupAvg(i))/ssRightForearmSupRange * 160;
    else
        rightForearmSup(i) = (ssRightForearmSupMax - ssRightForearmSup(i))/ssRightForearmSupRange * 160;
    end
end

%%
% 4: Plot results.
%

% Plot data.
figure
plot( time,rightElbowExt, time,rightForearmSup )
xlabel('Time (s)')
ylabel('Angle (deg)')
title('Elbow Mapping on Sample Data from April 23, 2015 (Title of Data Set)')
legend('show')
legend('Elbow Bending', 'Forearm Orientation', 'Location', 'southoutside');

