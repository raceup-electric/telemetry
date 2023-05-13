int data[12] = {};

void setup()
{
  Serial.begin(9600);
}

void loop()
{
	delay(random(50, 70));
  for (int t = 0; t < 4; t++)
    data[t] = random(20, 130);
  for (int hv = 4; hv < 7; hv++)
    data[hv] = random(3000, 4500);
  for (int lv = 7; lv < 9; lv++)
    data[lv] = random(11000, 12000);
  data[9] = random(1, 100);
  data[10] = random(-360, 360);
  data[11] = random(1, 100);
  String str = "";
  for (int i = 0; i < 12; i++) {
    str.concat(data[i]);
    if (i != 11)
      str.concat(",");
  }
  Serial.println(str);
}
