noise = true;
dT = .02;
[Acc,Mag,wGyro,Eul] = CreateTrajectoryData(dT,noise);
He = [22770;5329;41510.2]/1000;
Hu = He/norm(He);
g = [0;0;1];
pitches =[];
yaws =[];
rolls =[];

    [Rmis, pBody] = AlignPrimarySecondary(Acc',Mag',g,Hu);
    pitch = -asind(Rmis(1, 3))
    yaw = atan2d(Rmis(1,2),Rmis(1,1))
    roll = atan2d(Rmis(2,3),Rmis(3,3))
    pitches = [pitches;pitch];
    yaws = [yaws,yaw];
    rolls = [rolls,roll];
