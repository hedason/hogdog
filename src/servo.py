from src import control, config

class Servo:
    
    def __init__(self, name, channel): 
        self.name = name;
        self.channel = channel;

    def goto(self, pos):
        self.set(config.degStates[pos[0]])

    def goto_meat_mag(self):
        config.meat_curr = (config.meat_curr + 1) % len(config.meat_degs)
        pos = config.degStates[f"meat{config.meat_curr}"]
        print(pos)
        self.set(pos)

    def goto_veg_mag(self):
        config.veg_curr = (config.veg_curr + 1) % len(config.veg_degs)
        pos = config.degStates[f"veg{config.veg_curr}"]
        self.set(pos)

    def run(self, cw, timeout, gpio, throttle):
        control.Control.cservo(self, cw, timeout, gpio, throttle)

    def set(self, pos):
        control.Control.rservo(self, pos)

    def down(self):
        self.run(True, 0, 20, 1)

    def up(self):
        self.run(False, 0, 26, 1)

    def dress(self):
        self.run(True, 0.5, 0, 1)

    def stop():
        for s in servos:
            if not hasattr(servos[s], 'deg'):
                control.Control.stop(servos[s].channel);

class SServo(Servo):
    def __init__(self, pulse_min, pulse_max, deg, *args):
        self.deg = deg;
        self.pulse_min = pulse_min;
        self.pulse_max = pulse_max;
        super(SServo,self).__init__(*args)
        

class CServo(Servo):
    def __init__(self, *args):
        super(CServo,self).__init__(*args)

servos = {}

servos["d_arm"] = SServo(590, 1950, 75, "d_arm", 1)
servos["d_cyl"] = CServo("d_cyl", 0)

servos["b_arm"] = SServo(650, 1870, 165, "b_arm", 2)
servos["b_mag"] = CServo("b_mag", 3)
servos["dress_1"] = CServo("dress_1", 3)
servos["dress_2"] = CServo("dress_2", 4)