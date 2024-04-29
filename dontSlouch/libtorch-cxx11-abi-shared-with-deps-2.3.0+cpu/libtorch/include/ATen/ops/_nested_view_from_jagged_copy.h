#pragma once

// @generated by torchgen/gen.py from Function.h

#include <ATen/Context.h>
#include <ATen/DeviceGuard.h>
#include <ATen/TensorUtils.h>
#include <ATen/TracerMode.h>
#include <ATen/core/Generator.h>
#include <ATen/core/Reduction.h>
#include <ATen/core/Tensor.h>
#include <c10/core/Scalar.h>
#include <c10/core/Storage.h>
#include <c10/core/TensorOptions.h>
#include <c10/util/Deprecated.h>
#include <c10/util/Optional.h>



#include <ATen/ops/_nested_view_from_jagged_copy_ops.h>

namespace at {


// aten::_nested_view_from_jagged_copy(Tensor self, Tensor offsets, Tensor dummy, Tensor? lengths=None, int ragged_idx=1) -> Tensor
inline at::Tensor _nested_view_from_jagged_copy(const at::Tensor & self, const at::Tensor & offsets, const at::Tensor & dummy, const c10::optional<at::Tensor> & lengths={}, int64_t ragged_idx=1) {
    return at::_ops::_nested_view_from_jagged_copy::call(self, offsets, dummy, lengths, ragged_idx);
}

// aten::_nested_view_from_jagged_copy.out(Tensor self, Tensor offsets, Tensor dummy, Tensor? lengths=None, int ragged_idx=1, *, Tensor(a!) out) -> Tensor(a!)
inline at::Tensor & _nested_view_from_jagged_copy_out(at::Tensor & out, const at::Tensor & self, const at::Tensor & offsets, const at::Tensor & dummy, const c10::optional<at::Tensor> & lengths={}, int64_t ragged_idx=1) {
    return at::_ops::_nested_view_from_jagged_copy_out::call(self, offsets, dummy, lengths, ragged_idx, out);
}
// aten::_nested_view_from_jagged_copy.out(Tensor self, Tensor offsets, Tensor dummy, Tensor? lengths=None, int ragged_idx=1, *, Tensor(a!) out) -> Tensor(a!)
inline at::Tensor & _nested_view_from_jagged_copy_outf(const at::Tensor & self, const at::Tensor & offsets, const at::Tensor & dummy, const c10::optional<at::Tensor> & lengths, int64_t ragged_idx, at::Tensor & out) {
    return at::_ops::_nested_view_from_jagged_copy_out::call(self, offsets, dummy, lengths, ragged_idx, out);
}

}