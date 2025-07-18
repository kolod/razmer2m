#pragma message ( "\n\n \
	inverted B signals \
" )
yn_t gpio_B0isActive(){
	if(PINA & 1){
		return NO;
	}
	return YES;
}

yn_t gpio_B1isActive(){
	if(PINA & 2){
		return NO;
	}
	return YES;
}

yn_t gpio_B2isActive(){
	if(PINA & 4){
		return NO;
	}
	return YES;
}

yn_t gpio_B3isActive(){
	if(PINA & 8){
		return NO;
	}
	return YES;
}

yn_t gpio_B4isActive(){
	if(PINA & 16){
		return NO;
	}
	return YES;
}
