#pragma message ( "\n\n \
	inverted B signals \
" )

void gpio_B0(){
	digitalWrite(B0_PIN, LOW);
	digitalWrite(B1_PIN, HIGH);
	digitalWrite(B2_PIN, HIGH);
	digitalWrite(B3_PIN, HIGH);
	digitalWrite(B4_PIN, HIGH);
	digitalWrite(A7_PIN, HIGH);
}

void gpio_B1(){
	digitalWrite(B0_PIN, HIGH);
	digitalWrite(B1_PIN, LOW);
	digitalWrite(B2_PIN, HIGH);
	digitalWrite(B3_PIN, HIGH);
	digitalWrite(B4_PIN, HIGH);
	digitalWrite(A7_PIN, HIGH);
}

void gpio_B2(){
	digitalWrite(B0_PIN, HIGH);
	digitalWrite(B1_PIN, HIGH);
	digitalWrite(B2_PIN, LOW);
	digitalWrite(B3_PIN, HIGH);
	digitalWrite(B4_PIN, HIGH);
	digitalWrite(A7_PIN, HIGH);
}

void gpio_B3(){
	digitalWrite(B0_PIN, HIGH);
	digitalWrite(B1_PIN, HIGH);
	digitalWrite(B2_PIN, HIGH);
	digitalWrite(B3_PIN, LOW);
	digitalWrite(B4_PIN, HIGH);
	digitalWrite(A7_PIN, HIGH);
}

void gpio_B4(){
	digitalWrite(B0_PIN, HIGH);
	digitalWrite(B1_PIN, HIGH);
	digitalWrite(B2_PIN, HIGH);
	digitalWrite(B3_PIN, HIGH);
	digitalWrite(B4_PIN, LOW);
	digitalWrite(A7_PIN, HIGH);
}

void gpio_A7B0(){
	digitalWrite(B0_PIN, LOW);
	digitalWrite(B1_PIN, HIGH);
	digitalWrite(B2_PIN, HIGH);
	digitalWrite(B3_PIN, HIGH);
	digitalWrite(B4_PIN, HIGH);
	digitalWrite(A7_PIN, LOW);
}

void gpio_A7B1(){
	digitalWrite(B0_PIN, HIGH);
	digitalWrite(B1_PIN, LOW);
	digitalWrite(B2_PIN, HIGH);
	digitalWrite(B3_PIN, HIGH);
	digitalWrite(B4_PIN, HIGH);
	digitalWrite(A7_PIN, LOW);
}

void gpio_A7B2(){
	digitalWrite(B0_PIN, HIGH);
	digitalWrite(B1_PIN, HIGH);
	digitalWrite(B2_PIN, LOW);
	digitalWrite(B3_PIN, HIGH);
	digitalWrite(B4_PIN, HIGH);
	digitalWrite(A7_PIN, LOW);
}

void gpio_A7B3(){
	digitalWrite(B0_PIN, HIGH);
	digitalWrite(B1_PIN, HIGH);
	digitalWrite(B2_PIN, HIGH);
	digitalWrite(B3_PIN, LOW);
	digitalWrite(B4_PIN, HIGH);
	digitalWrite(A7_PIN, LOW);
}

void gpio_A7B4(){
	digitalWrite(B0_PIN, HIGH);
	digitalWrite(B1_PIN, HIGH);
	digitalWrite(B2_PIN, HIGH);
	digitalWrite(B3_PIN, HIGH);
	digitalWrite(B4_PIN, LOW);
	digitalWrite(A7_PIN, LOW);
}
