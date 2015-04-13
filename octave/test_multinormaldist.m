meanv = [0, 0]; covm = eye(2);

covm = [ 1 0.8; 0.8 1 ];

X = linspace(-5, 5, 50); Y = linspace(-5, 5, 50);
Z = meshgrid(X, Y);
a = 1; b = 1; % WTF, 
for i = X,
	b = 1;
	for j = Y,
		% printf('%d %d %f %f\n', a, b, i, j);
		Z(a, b) = multivar_normal_dist([j, i], meanv, covm);
		b = b + 1;
	end
	a = a + 1;
end