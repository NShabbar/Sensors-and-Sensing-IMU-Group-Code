%% Part 4.1 Integration with Open-Loop
clc;
clear; 
close all;

% Step 1: Generate trajectory data without noise
dT = 1/50; % Set timestep to match real data (e.g., 50Hz)
noiseFlag = false; % No noise

[Acc, Mag, wGyro, Eul_true] = CreateTrajectoryData(dT, noiseFlag);

yaw_100 = wGyro(1:50,1);
pitch_100 = wGyro(1:50,2);
roll_100 = wGyro(1:50,3);

yaw_bias = mean(yaw_100);
pitch_bias = mean(pitch_100);
roll_bias = mean(roll_100);

% Step 2: Extract true Euler angles
yaw_true = deg2rad(Eul_true(:, 1));
pitch_true = deg2rad(Eul_true(:, 2));
roll_true = deg2rad(Eul_true(:, 3));

% Step 3: Initialize DCM from true Euler angles
R_0 = [(cos(pitch_true(1))*cos(yaw_true(1))), cos(pitch_true(1))*sin(yaw_true(1)), -sin(pitch_true(1));
        (sin(roll_true(1))*sin(pitch_true(1))*cos(yaw_true(1)))-(cos(roll_true(1))*sin(yaw_true(1))), (sin(roll_true(1))*sin(pitch_true(1))*sin(yaw_true(1))) + (cos(roll_true(1))*cos(yaw_true(1))), sin(roll_true(1))*cos(pitch_true(1));
        (cos(roll_true(1))*sin(pitch_true(1))*cos(yaw_true(1)))+ (sin(roll_true(1))*sin(yaw_true(1))), (cos(roll_true(1))*sin(pitch_true(1))*sin(yaw_true(1))) - (sin(roll_true(1))*cos(yaw_true(1))), cos(roll_true(1))*cos(pitch_true(1))];

% Step 4: Integrate gyroscope output using forward integration and matrix exponential form
pitches =[];
yaws = [];
rolls = [];
for i =1:length(pitch_true)
    yaw_test = wGyro(i,1);
    pitch_test = wGyro(i,2);
    roll_test = wGyro(i,3);
    if i >= 50
        yaw_test = yaw_test - yaw_bias;
        pitch_test = pitch_test - pitch_bias;
        roll_test = roll_test -roll_bias;
    end
    foo = [deg2rad(yaw_test/131),deg2rad(pitch_test/131),deg2rad(roll_test/131)];
    if i == 1
        Rk_1 = IntegrateOpenLoop(R_0,foo, dT);
    else
        Rk_1 = IntegrateOpenLoop(Rk_1,foo,dT);
    end
    pitch = -asin(Rk_1(1, 3));
    yaw = atan2(Rk_1(1,2),Rk_1(1,1));
    roll = atan2(Rk_1(2,3),Rk_1(3,3));
    pitches = [pitches; pitch];
    yaws = [yaws; yaw];
    rolls = [rolls; roll];
end
% Step 6: Compare calculated Euler angles with true Euler angles and plot errors
figure(1);

subplot(3, 1, 1);
plot(rad2deg(yaw_true) - rad2deg(yaws));
title('Yaw Error (Forward Integration)');

subplot(3, 1, 2);
plot((rad2deg(pitch_true) - rad2deg(pitches)));
title('Pitch Error (Forward Integration)');

subplot(3, 1, 3);
plot((rad2deg(roll_true) - rad2deg(rolls)));
title('Roll Error (Forward Integration)');


