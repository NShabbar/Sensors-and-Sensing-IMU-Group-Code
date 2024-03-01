To fix the problem of the complex numbers from pitch in part 4, we added the code 
    if Rk_1(1,3) > 1
        Rk_1(1,3) = .9999;
    elseif Rk_1(1,3) < -1
        Rk_1(1,3) = -.9999;
    end
	
Because sometimes the forward integration returned values outside of the range [-1,1] which seems like it should be impossible. So we just hard code it to be in range
