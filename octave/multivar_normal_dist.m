function [prob] = multivar_normal_dist(x, meanv, covm)

	n = length(meanv);
	offset = x - meanv;
	prob = exp(-0.5 * (offset)*inv(covm)*(offset')) * (1 / ((2*pi)^(n/2)*(det(covm)^0.5)));


end