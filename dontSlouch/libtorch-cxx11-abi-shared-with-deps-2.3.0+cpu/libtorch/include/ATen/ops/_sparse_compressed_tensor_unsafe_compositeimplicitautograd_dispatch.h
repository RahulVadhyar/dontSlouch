#pragma once
// @generated by torchgen/gen.py from DispatchKeyFunction.h

// NB: The implementing C++ file is RegisterDispatchKey.cpp

// The only #includes we need are for custom classes that have defaults in the C++ API
#include <c10/core/MemoryFormat.h>
#include <c10/core/Scalar.h>
#include <ATen/core/Reduction.h>

// Forward declarations of any types needed in the operator signatures.
// We can't directly include these classes because it will cause circular include dependencies.
// This file is included by TensorBody.h, which defines the Tensor class.
#include <ATen/core/ATen_fwd.h>

namespace at {

namespace compositeimplicitautograd {

TORCH_API at::Tensor _sparse_compressed_tensor_unsafe(const at::Tensor & compressed_indices, const at::Tensor & plain_indices, const at::Tensor & values, at::IntArrayRef size, at::TensorOptions options={});
TORCH_API at::Tensor _sparse_compressed_tensor_unsafe(const at::Tensor & compressed_indices, const at::Tensor & plain_indices, const at::Tensor & values, at::IntArrayRef size, c10::optional<at::ScalarType> dtype, c10::optional<at::Layout> layout, c10::optional<at::Device> device, c10::optional<bool> pin_memory);
TORCH_API at::Tensor _sparse_compressed_tensor_unsafe_symint(const at::Tensor & compressed_indices, const at::Tensor & plain_indices, const at::Tensor & values, c10::SymIntArrayRef size, at::TensorOptions options={});
TORCH_API at::Tensor _sparse_compressed_tensor_unsafe_symint(const at::Tensor & compressed_indices, const at::Tensor & plain_indices, const at::Tensor & values, c10::SymIntArrayRef size, c10::optional<at::ScalarType> dtype, c10::optional<at::Layout> layout, c10::optional<at::Device> device, c10::optional<bool> pin_memory);

} // namespace compositeimplicitautograd
} // namespace at