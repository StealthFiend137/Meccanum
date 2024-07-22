class dpad_handler:
                    
    def __init__(self, x, y, callback):
        self.x = x
        self.y = y
        self.callback = callback
        
        self.response = ""
        
        x.set_callback = self.callback_func
        y.set_callback = self.callback_func
        
    def callback_func(self, result):
        self.response = self.response + result;
        
    def handle_event(self, event):
        self.response = ""
        self.x.handle_event(event)
        self.y.handle_event(event)
        if self.response != "":
            self.callback(self.response)
            
if __name__ == '__main__':
    print('DpadHandler should not be run directly.')
        
        
class dpad_axis_handler:
    def __init__(self, axis_name, axis_code):
        self.axis_name = axis_name
        self.axis_code = axis_code
        
    def set_callback(self, callback):
        self.callback = callback
    
    def handle_event(self, event):
        print(f'handling event {self.axis_name}')
        if event.code == self.axis_name:
            result = f'{self.axis_name}: 1'
            self.callback(result)