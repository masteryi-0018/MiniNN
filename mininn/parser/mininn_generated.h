// automatically generated by the FlatBuffers compiler, do not modify


#ifndef FLATBUFFERS_GENERATED_MININN_MININN_FBS_H_
#define FLATBUFFERS_GENERATED_MININN_MININN_FBS_H_

#include "flatbuffers/flatbuffers.h"

// Ensure the included flatbuffers.h is the same version as when this file was
// generated, otherwise it may not be compatible.
static_assert(FLATBUFFERS_VERSION_MAJOR == 25 &&
              FLATBUFFERS_VERSION_MINOR == 2 &&
              FLATBUFFERS_VERSION_REVISION == 10,
             "Non-compatible flatbuffers version included");

namespace mininn_fbs {

struct Attribute;
struct AttributeBuilder;

struct Node;
struct NodeBuilder;

struct Tensor;
struct TensorBuilder;

struct Graph;
struct GraphBuilder;

enum Op : int8_t {
  Op_ADD = 0,
  Op_CONV = 1,
  Op_CLIP = 2,
  Op_SHAPE = 3,
  Op_GATHER = 4,
  Op_UNSQUEEZE = 5,
  Op_CONCAT = 6,
  Op_GLOBALAVERAGEPOOL = 7,
  Op_RESHAPE = 8,
  Op_GEMM = 9,
  Op_CONSTANT = 10,
  Op_MIN = Op_ADD,
  Op_MAX = Op_CONSTANT
};

inline const Op (&EnumValuesOp())[11] {
  static const Op values[] = {
    Op_ADD,
    Op_CONV,
    Op_CLIP,
    Op_SHAPE,
    Op_GATHER,
    Op_UNSQUEEZE,
    Op_CONCAT,
    Op_GLOBALAVERAGEPOOL,
    Op_RESHAPE,
    Op_GEMM,
    Op_CONSTANT
  };
  return values;
}

inline const char * const *EnumNamesOp() {
  static const char * const names[12] = {
    "ADD",
    "CONV",
    "CLIP",
    "SHAPE",
    "GATHER",
    "UNSQUEEZE",
    "CONCAT",
    "GLOBALAVERAGEPOOL",
    "RESHAPE",
    "GEMM",
    "CONSTANT",
    nullptr
  };
  return names;
}

inline const char *EnumNameOp(Op e) {
  if (::flatbuffers::IsOutRange(e, Op_ADD, Op_CONSTANT)) return "";
  const size_t index = static_cast<size_t>(e);
  return EnumNamesOp()[index];
}

struct Attribute FLATBUFFERS_FINAL_CLASS : private ::flatbuffers::Table {
  typedef AttributeBuilder Builder;
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_KEY = 4,
    VT_VALUE = 6
  };
  const ::flatbuffers::String *key() const {
    return GetPointer<const ::flatbuffers::String *>(VT_KEY);
  }
  const ::flatbuffers::Vector<int32_t> *value() const {
    return GetPointer<const ::flatbuffers::Vector<int32_t> *>(VT_VALUE);
  }
  bool Verify(::flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyOffset(verifier, VT_KEY) &&
           verifier.VerifyString(key()) &&
           VerifyOffset(verifier, VT_VALUE) &&
           verifier.VerifyVector(value()) &&
           verifier.EndTable();
  }
};

struct AttributeBuilder {
  typedef Attribute Table;
  ::flatbuffers::FlatBufferBuilder &fbb_;
  ::flatbuffers::uoffset_t start_;
  void add_key(::flatbuffers::Offset<::flatbuffers::String> key) {
    fbb_.AddOffset(Attribute::VT_KEY, key);
  }
  void add_value(::flatbuffers::Offset<::flatbuffers::Vector<int32_t>> value) {
    fbb_.AddOffset(Attribute::VT_VALUE, value);
  }
  explicit AttributeBuilder(::flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  ::flatbuffers::Offset<Attribute> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = ::flatbuffers::Offset<Attribute>(end);
    return o;
  }
};

inline ::flatbuffers::Offset<Attribute> CreateAttribute(
    ::flatbuffers::FlatBufferBuilder &_fbb,
    ::flatbuffers::Offset<::flatbuffers::String> key = 0,
    ::flatbuffers::Offset<::flatbuffers::Vector<int32_t>> value = 0) {
  AttributeBuilder builder_(_fbb);
  builder_.add_value(value);
  builder_.add_key(key);
  return builder_.Finish();
}

inline ::flatbuffers::Offset<Attribute> CreateAttributeDirect(
    ::flatbuffers::FlatBufferBuilder &_fbb,
    const char *key = nullptr,
    const std::vector<int32_t> *value = nullptr) {
  auto key__ = key ? _fbb.CreateString(key) : 0;
  auto value__ = value ? _fbb.CreateVector<int32_t>(*value) : 0;
  return mininn_fbs::CreateAttribute(
      _fbb,
      key__,
      value__);
}

struct Node FLATBUFFERS_FINAL_CLASS : private ::flatbuffers::Table {
  typedef NodeBuilder Builder;
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_TYPE = 4,
    VT_ATTRIBUTES = 6,
    VT_INPUTS = 8,
    VT_OUTPUTS = 10
  };
  mininn_fbs::Op type() const {
    return static_cast<mininn_fbs::Op>(GetField<int8_t>(VT_TYPE, 0));
  }
  const ::flatbuffers::Vector<::flatbuffers::Offset<mininn_fbs::Attribute>> *attributes() const {
    return GetPointer<const ::flatbuffers::Vector<::flatbuffers::Offset<mininn_fbs::Attribute>> *>(VT_ATTRIBUTES);
  }
  const ::flatbuffers::Vector<int32_t> *inputs() const {
    return GetPointer<const ::flatbuffers::Vector<int32_t> *>(VT_INPUTS);
  }
  const ::flatbuffers::Vector<int32_t> *outputs() const {
    return GetPointer<const ::flatbuffers::Vector<int32_t> *>(VT_OUTPUTS);
  }
  bool Verify(::flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyField<int8_t>(verifier, VT_TYPE, 1) &&
           VerifyOffset(verifier, VT_ATTRIBUTES) &&
           verifier.VerifyVector(attributes()) &&
           verifier.VerifyVectorOfTables(attributes()) &&
           VerifyOffset(verifier, VT_INPUTS) &&
           verifier.VerifyVector(inputs()) &&
           VerifyOffset(verifier, VT_OUTPUTS) &&
           verifier.VerifyVector(outputs()) &&
           verifier.EndTable();
  }
};

struct NodeBuilder {
  typedef Node Table;
  ::flatbuffers::FlatBufferBuilder &fbb_;
  ::flatbuffers::uoffset_t start_;
  void add_type(mininn_fbs::Op type) {
    fbb_.AddElement<int8_t>(Node::VT_TYPE, static_cast<int8_t>(type), 0);
  }
  void add_attributes(::flatbuffers::Offset<::flatbuffers::Vector<::flatbuffers::Offset<mininn_fbs::Attribute>>> attributes) {
    fbb_.AddOffset(Node::VT_ATTRIBUTES, attributes);
  }
  void add_inputs(::flatbuffers::Offset<::flatbuffers::Vector<int32_t>> inputs) {
    fbb_.AddOffset(Node::VT_INPUTS, inputs);
  }
  void add_outputs(::flatbuffers::Offset<::flatbuffers::Vector<int32_t>> outputs) {
    fbb_.AddOffset(Node::VT_OUTPUTS, outputs);
  }
  explicit NodeBuilder(::flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  ::flatbuffers::Offset<Node> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = ::flatbuffers::Offset<Node>(end);
    return o;
  }
};

inline ::flatbuffers::Offset<Node> CreateNode(
    ::flatbuffers::FlatBufferBuilder &_fbb,
    mininn_fbs::Op type = mininn_fbs::Op_ADD,
    ::flatbuffers::Offset<::flatbuffers::Vector<::flatbuffers::Offset<mininn_fbs::Attribute>>> attributes = 0,
    ::flatbuffers::Offset<::flatbuffers::Vector<int32_t>> inputs = 0,
    ::flatbuffers::Offset<::flatbuffers::Vector<int32_t>> outputs = 0) {
  NodeBuilder builder_(_fbb);
  builder_.add_outputs(outputs);
  builder_.add_inputs(inputs);
  builder_.add_attributes(attributes);
  builder_.add_type(type);
  return builder_.Finish();
}

inline ::flatbuffers::Offset<Node> CreateNodeDirect(
    ::flatbuffers::FlatBufferBuilder &_fbb,
    mininn_fbs::Op type = mininn_fbs::Op_ADD,
    const std::vector<::flatbuffers::Offset<mininn_fbs::Attribute>> *attributes = nullptr,
    const std::vector<int32_t> *inputs = nullptr,
    const std::vector<int32_t> *outputs = nullptr) {
  auto attributes__ = attributes ? _fbb.CreateVector<::flatbuffers::Offset<mininn_fbs::Attribute>>(*attributes) : 0;
  auto inputs__ = inputs ? _fbb.CreateVector<int32_t>(*inputs) : 0;
  auto outputs__ = outputs ? _fbb.CreateVector<int32_t>(*outputs) : 0;
  return mininn_fbs::CreateNode(
      _fbb,
      type,
      attributes__,
      inputs__,
      outputs__);
}

struct Tensor FLATBUFFERS_FINAL_CLASS : private ::flatbuffers::Table {
  typedef TensorBuilder Builder;
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_SHAPE = 4,
    VT_DATA = 6
  };
  const ::flatbuffers::Vector<int32_t> *shape() const {
    return GetPointer<const ::flatbuffers::Vector<int32_t> *>(VT_SHAPE);
  }
  const ::flatbuffers::Vector<uint8_t> *data() const {
    return GetPointer<const ::flatbuffers::Vector<uint8_t> *>(VT_DATA);
  }
  bool Verify(::flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyOffset(verifier, VT_SHAPE) &&
           verifier.VerifyVector(shape()) &&
           VerifyOffset(verifier, VT_DATA) &&
           verifier.VerifyVector(data()) &&
           verifier.EndTable();
  }
};

struct TensorBuilder {
  typedef Tensor Table;
  ::flatbuffers::FlatBufferBuilder &fbb_;
  ::flatbuffers::uoffset_t start_;
  void add_shape(::flatbuffers::Offset<::flatbuffers::Vector<int32_t>> shape) {
    fbb_.AddOffset(Tensor::VT_SHAPE, shape);
  }
  void add_data(::flatbuffers::Offset<::flatbuffers::Vector<uint8_t>> data) {
    fbb_.AddOffset(Tensor::VT_DATA, data);
  }
  explicit TensorBuilder(::flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  ::flatbuffers::Offset<Tensor> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = ::flatbuffers::Offset<Tensor>(end);
    return o;
  }
};

inline ::flatbuffers::Offset<Tensor> CreateTensor(
    ::flatbuffers::FlatBufferBuilder &_fbb,
    ::flatbuffers::Offset<::flatbuffers::Vector<int32_t>> shape = 0,
    ::flatbuffers::Offset<::flatbuffers::Vector<uint8_t>> data = 0) {
  TensorBuilder builder_(_fbb);
  builder_.add_data(data);
  builder_.add_shape(shape);
  return builder_.Finish();
}

inline ::flatbuffers::Offset<Tensor> CreateTensorDirect(
    ::flatbuffers::FlatBufferBuilder &_fbb,
    const std::vector<int32_t> *shape = nullptr,
    const std::vector<uint8_t> *data = nullptr) {
  auto shape__ = shape ? _fbb.CreateVector<int32_t>(*shape) : 0;
  auto data__ = data ? _fbb.CreateVector<uint8_t>(*data) : 0;
  return mininn_fbs::CreateTensor(
      _fbb,
      shape__,
      data__);
}

struct Graph FLATBUFFERS_FINAL_CLASS : private ::flatbuffers::Table {
  typedef GraphBuilder Builder;
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_NODES = 4,
    VT_TENSORS = 6,
    VT_INPUTS = 8,
    VT_OUTPUTS = 10
  };
  const ::flatbuffers::Vector<::flatbuffers::Offset<mininn_fbs::Node>> *nodes() const {
    return GetPointer<const ::flatbuffers::Vector<::flatbuffers::Offset<mininn_fbs::Node>> *>(VT_NODES);
  }
  const ::flatbuffers::Vector<::flatbuffers::Offset<mininn_fbs::Tensor>> *tensors() const {
    return GetPointer<const ::flatbuffers::Vector<::flatbuffers::Offset<mininn_fbs::Tensor>> *>(VT_TENSORS);
  }
  const ::flatbuffers::Vector<int32_t> *inputs() const {
    return GetPointer<const ::flatbuffers::Vector<int32_t> *>(VT_INPUTS);
  }
  const ::flatbuffers::Vector<int32_t> *outputs() const {
    return GetPointer<const ::flatbuffers::Vector<int32_t> *>(VT_OUTPUTS);
  }
  bool Verify(::flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyOffset(verifier, VT_NODES) &&
           verifier.VerifyVector(nodes()) &&
           verifier.VerifyVectorOfTables(nodes()) &&
           VerifyOffset(verifier, VT_TENSORS) &&
           verifier.VerifyVector(tensors()) &&
           verifier.VerifyVectorOfTables(tensors()) &&
           VerifyOffset(verifier, VT_INPUTS) &&
           verifier.VerifyVector(inputs()) &&
           VerifyOffset(verifier, VT_OUTPUTS) &&
           verifier.VerifyVector(outputs()) &&
           verifier.EndTable();
  }
};

struct GraphBuilder {
  typedef Graph Table;
  ::flatbuffers::FlatBufferBuilder &fbb_;
  ::flatbuffers::uoffset_t start_;
  void add_nodes(::flatbuffers::Offset<::flatbuffers::Vector<::flatbuffers::Offset<mininn_fbs::Node>>> nodes) {
    fbb_.AddOffset(Graph::VT_NODES, nodes);
  }
  void add_tensors(::flatbuffers::Offset<::flatbuffers::Vector<::flatbuffers::Offset<mininn_fbs::Tensor>>> tensors) {
    fbb_.AddOffset(Graph::VT_TENSORS, tensors);
  }
  void add_inputs(::flatbuffers::Offset<::flatbuffers::Vector<int32_t>> inputs) {
    fbb_.AddOffset(Graph::VT_INPUTS, inputs);
  }
  void add_outputs(::flatbuffers::Offset<::flatbuffers::Vector<int32_t>> outputs) {
    fbb_.AddOffset(Graph::VT_OUTPUTS, outputs);
  }
  explicit GraphBuilder(::flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  ::flatbuffers::Offset<Graph> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = ::flatbuffers::Offset<Graph>(end);
    return o;
  }
};

inline ::flatbuffers::Offset<Graph> CreateGraph(
    ::flatbuffers::FlatBufferBuilder &_fbb,
    ::flatbuffers::Offset<::flatbuffers::Vector<::flatbuffers::Offset<mininn_fbs::Node>>> nodes = 0,
    ::flatbuffers::Offset<::flatbuffers::Vector<::flatbuffers::Offset<mininn_fbs::Tensor>>> tensors = 0,
    ::flatbuffers::Offset<::flatbuffers::Vector<int32_t>> inputs = 0,
    ::flatbuffers::Offset<::flatbuffers::Vector<int32_t>> outputs = 0) {
  GraphBuilder builder_(_fbb);
  builder_.add_outputs(outputs);
  builder_.add_inputs(inputs);
  builder_.add_tensors(tensors);
  builder_.add_nodes(nodes);
  return builder_.Finish();
}

inline ::flatbuffers::Offset<Graph> CreateGraphDirect(
    ::flatbuffers::FlatBufferBuilder &_fbb,
    const std::vector<::flatbuffers::Offset<mininn_fbs::Node>> *nodes = nullptr,
    const std::vector<::flatbuffers::Offset<mininn_fbs::Tensor>> *tensors = nullptr,
    const std::vector<int32_t> *inputs = nullptr,
    const std::vector<int32_t> *outputs = nullptr) {
  auto nodes__ = nodes ? _fbb.CreateVector<::flatbuffers::Offset<mininn_fbs::Node>>(*nodes) : 0;
  auto tensors__ = tensors ? _fbb.CreateVector<::flatbuffers::Offset<mininn_fbs::Tensor>>(*tensors) : 0;
  auto inputs__ = inputs ? _fbb.CreateVector<int32_t>(*inputs) : 0;
  auto outputs__ = outputs ? _fbb.CreateVector<int32_t>(*outputs) : 0;
  return mininn_fbs::CreateGraph(
      _fbb,
      nodes__,
      tensors__,
      inputs__,
      outputs__);
}

inline const mininn_fbs::Graph *GetGraph(const void *buf) {
  return ::flatbuffers::GetRoot<mininn_fbs::Graph>(buf);
}

inline const mininn_fbs::Graph *GetSizePrefixedGraph(const void *buf) {
  return ::flatbuffers::GetSizePrefixedRoot<mininn_fbs::Graph>(buf);
}

inline bool VerifyGraphBuffer(
    ::flatbuffers::Verifier &verifier) {
  return verifier.VerifyBuffer<mininn_fbs::Graph>(nullptr);
}

inline bool VerifySizePrefixedGraphBuffer(
    ::flatbuffers::Verifier &verifier) {
  return verifier.VerifySizePrefixedBuffer<mininn_fbs::Graph>(nullptr);
}

inline void FinishGraphBuffer(
    ::flatbuffers::FlatBufferBuilder &fbb,
    ::flatbuffers::Offset<mininn_fbs::Graph> root) {
  fbb.Finish(root);
}

inline void FinishSizePrefixedGraphBuffer(
    ::flatbuffers::FlatBufferBuilder &fbb,
    ::flatbuffers::Offset<mininn_fbs::Graph> root) {
  fbb.FinishSizePrefixed(root);
}

}  // namespace mininn_fbs

#endif  // FLATBUFFERS_GENERATED_MININN_MININN_FBS_H_
