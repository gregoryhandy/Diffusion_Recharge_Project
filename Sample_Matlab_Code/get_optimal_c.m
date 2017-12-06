%% Calculates the optimal C for 1D interval
function [ C,T,temp ] = get_optimal_c

num_terms = 5000;
lambda = zeros(num_terms,1);
A = zeros(num_terms,1);
for k = 0:num_terms
    lambda(k+1) = ((pi+2*pi*k)/(2))^2;
    A(k+1) = 4/(pi+2*pi*k);
end

T_star=10;
T = [0:.001:T_star];
temp = zeros(1,size(T,1));
for k = 0:num_terms
    % if uniform, use 2/(pi*(2*k+1)) instead of (sin(sqrt(lambda(k+1))*0.5));
    temp = temp + A(k+1)*(sin(sqrt(lambda(k+1))*0.5))*exp(-(lambda(k+1)-lambda(1))*T);
end
C = max(temp);

%Produce graph to debug
% figure;
% plot(T,temp.*exp(-lambda(1)*T))
% hold on;
% plot(T, C*exp(-lambda(1)*T),'black')
% xlabel('T')
% ylabel('Survivial Probability')
% legend('Exact', 'Upper Bound')
end






