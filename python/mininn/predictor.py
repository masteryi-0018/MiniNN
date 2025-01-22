import mininn

class Predictor():
    def __init__(self, filename):
        graph = mininn.Graph()
        mininn.load_model(filename, graph)
        self.Predictor = mininn.Predictor(graph)

    def get_input(self):
        inputs = self.Predictor.get_input_tensors()
        return inputs

    def set_data(self, a, b):
        input = self.Predictor.get_input_tensors()
        input[0].set_data(a)
        input[1].set_data(b)

    def run(self):
        self.Predictor.prepare()
        self.Predictor.run()

    def get_output(self):
        outputs = self.Predictor.get_output_tensors()
        return outputs