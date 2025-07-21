#pragma message ( "\n\n \
	inverted B signals \
" )

yn_t gpio_B0isActive(){
	if(digitalRead(B0_PIN) == LOW){
		return YES;
	}
	return NO;
}

yn_t gpio_B1isActive(){
	if(digitalRead(B1_PIN) == LOW){
		return YES;
	}
	return NO;
}

yn_t gpio_B2isActive(){
	if(digitalRead(B2_PIN) == LOW){
		return YES;
	}
	return NO;
}

yn_t gpio_B3isActive(){
	if(digitalRead(B3_PIN) == LOW){
		return YES;
	}
	return NO;
}

yn_t gpio_B4isActive(){
	if(digitalRead(B4_PIN) == LOW){
		return YES;
	}
	return NO;
}
