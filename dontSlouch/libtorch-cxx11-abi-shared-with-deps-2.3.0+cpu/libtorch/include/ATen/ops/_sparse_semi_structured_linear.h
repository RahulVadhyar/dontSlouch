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



#include <ATen/ops/_sparse_semi_structured_linear_ops.h>

namespace at {


// aten::_sparse_semi_structured_linear(Tensor input, Tensor weight, Tensor meta, *, Tensor? bias=None, str? activation=None, ScalarType? out_dtype=None) -> Tensor
inline at::Tensor _sparse_semi_structured_linear(const at::Tensor & input, const at::Tensor & weight, const at::Tensor & meta, const c10::optional<at::Tensor> & bias={}, c10::optional<c10::string_view> activation=c10::nullopt, c10::optional<at::ScalarType> out_dtype=c10::nullopt) {
    return at::_ops::_sparse_semi_structured_linear::call(input, weight, meta, bias, activation, out_dtype);
}

}