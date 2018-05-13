clf;close all;
filename = 'dataFile/SensorData_60s.txt';
delimiterIn = ' ';
headerlinesIn = 0;
Data = importdata(filename,delimiterIn,headerlinesIn);

%Time
T(1) = Data(1,1);
for i = 2:size(Data(:,1))
    T(i) = T(i-1) + Data(i,1);
end

%Convert Acc Data to Angles(DEG)
theta_acc =  rad2deg(atan(Data(:,2)./sqrt(Data(:,3).^2 + (1.+Data(:,4)).^2)));
phi_acc =  rad2deg(atan(Data(:,3)./(1+Data(:,4))));

%Gyro Data (já está em DEG)
p = Data(:,5);
q = -Data(:,6);
r = Data(:,7);

%Filter Gains
K1 = 0.02;
K2 = 1/1000;

%PHI
phi = phi_acc(1) + p(1)*Data(1,1);
dp = 0;
for i = 2:size(p)
    p_c = p(i) + dp(i-1);
    phi_gy = phi(i-1) + p_c*Data(i,1);
    phi(i) =  phi_gy + K1*(phi_acc(i) - phi_gy);
    dp(i) = dp(i-1) + K2*(phi_acc(i) - phi_gy);
end

%THETA
theta = theta_acc (1) + q(1)*Data(1,1);
dq = 0;
for i = 2:size(p)
    q_c = q(i) + dq;
    theta_gy = theta(i-1) + q_c*Data(i,1);
    theta(i) =  theta_gy + K1*(theta_acc(i) - theta_gy);
    dq = dq + K2*(theta_acc(i) - theta_gy);
end

%r - recursive average with forgetting factor
% r_av = r(1)*ones(size(r));
% ff = 0.8;
% for i=2:length(r)
%    r_av(i) =  (r(i)*(1-ff) + r_av(i-1)*(ff-ff^(i-1)))/(1-ff^(i-1));
% end
%r - moving average filter
N = 15;
r_av = ones(size(r))*r(1);
r_N = ones(1,N)*r(1);
point_filt = 1;
for i = 2:length(r)
    
    r_av(i) = (r_av(i-1)*N-r_N(point_filt)+r(i))/N;
    r_N(point_filt) = r(i);
    
    if point_filt == N
        point_filt = 1;
    else
        point_filt = point_filt+1;
    end
end

clf;
subplot(1,3,1);
plot(T,phi_acc,'r');hold on;
plot(T,p,'b');
plot(T,phi,'k','LineWidth',1.5);
title('Roll - \phi');
legend('\phi - Acc','p - Gyro','\phi - Filtered');
xlabel('T [s]');ylabel('Deg / Deg s^{-1}');

subplot(1,3,2);
plot(T,theta_acc,'r');hold on;
plot(T,q,'b');
plot(T,theta,'k','LineWidth',1.5);
title('Pitch - \theta');
legend('\theta - acc','q - Gyro','\theta - Filtered');
xlabel('T [s]');ylabel('Deg / Deg s^{-1}');

subplot(1,3,3);
plot(T,r,'b');hold on;
plot(T,r_av,'k','LineWidth',1.5);
title('Yaw - \psi');
legend('r - Gyro','r - Filtered');
xlabel('T [s]');ylabel('Deg s^{-1}');


