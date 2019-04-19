// 2019.4.19 ————ROVmaker卖的无刷电机电调测试————

#define PWM_PIN 11

int def_p = 0;
int pos = 5;
void setup() {
	pinMode(PWM_PIN, OUTPUT);
	Serial.begin(9600);
	Serial.println("rootming");
	delay(100);
	analogWrite(PWM_PIN, 0);
	// set p to 0
	delay(100);
}

void loop() {
	analogWrite(PWM_PIN, def_p);
	def_p += pos;
	if(def_p == 255)
		pos = -5;
	if(def_p == 0)
		pos = 5;
	delay(100);
}
