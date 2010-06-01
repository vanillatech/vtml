//**********
//Member functions Layer
//**********


Layer::Layer(void) {
		this->step = 1;
		this->aqueue = new ActivationQueue(this);
		this->recQueue = new RecoveryQueue(this);
}

Layer::Layer(Layer *caller) {
		this->step = 1;
		this->aqueue = new ActivationQueue(this);
		this->recQueue = new RecoveryQueue(this);
		this->lower = caller;
}

void Layer::newStep(void) {
	this->step++;
	this->recQueue->recover();
	Debug1->ListBox1->Items->Insert(0,AnsiString("----------- ") + AnsiString(this->step) + AnsiString("------------"));
}
