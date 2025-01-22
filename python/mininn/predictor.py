import mininn

class predictor():
    def __init__(self, filename):
        graph = mininn.Graph()
        mininn.load_model(filename, graph)
        self.predictor = mininn.Predictor(graph)

    def get_input(self):
        inputs = self.predictor.get_input_tensors()
        return inputs

    def set_data(self, a, b):
        input = self.predictor.get_input_tensors()
        input[0].set_data(a)
        input[1].set_data(b)

    def run(self):
        self.predictor.prepare()
        self.predictor.run()

    def get_output(self):
        outputs = self.predictor.get_output_tensors()
        return outputs