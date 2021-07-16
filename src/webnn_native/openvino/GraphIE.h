// Copyright 2021 The WebNN-native Authors
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef WEBNN_NATIVE_IE_MODEL_IE_H_
#define WEBNN_NATIVE_IE_MODEL_IE_H_

#include <ie_nn_c_api.h>
#include <map>
#include <set>
#include <unordered_set>

#include "webnn_native/Error.h"
#include "webnn_native/Graph.h"
#include "webnn_native/Operand.h"
#include "webnn_native/openvino/ContextIE.h"
#include "webnn_native/ops/BatchNorm.h"
#include "webnn_native/ops/Binary.h"
#include "webnn_native/ops/Clamp.h"
#include "webnn_native/ops/Concat.h"
#include "webnn_native/ops/Constant.h"
#include "webnn_native/ops/Conv2d.h"
#include "webnn_native/ops/Gemm.h"
#include "webnn_native/ops/Input.h"
#include "webnn_native/ops/InstanceNorm.h"
#include "webnn_native/ops/LeakyRelu.h"
#include "webnn_native/ops/Pad.h"
#include "webnn_native/ops/Pool2d.h"
#include "webnn_native/ops/ReduceMean.h"
#include "webnn_native/ops/Reshape.h"
#include "webnn_native/ops/Transpose.h"
#include "webnn_native/ops/Unary.h"

namespace webnn_native { namespace ie {

    class Graph : public GraphBase {
      public:
        explicit Graph(Context* context);
        ~Graph() override;

        virtual MaybeError AddConstant(const op::Constant* constant) override;
        virtual MaybeError AddInput(const op::Input* input) override;
        virtual MaybeError AddOutput(const std::string& name, const OperandBase* ouput) override;
        virtual MaybeError AddBatchNorm(const op::BatchNorm* batchNorm) override;
        virtual MaybeError AddBinary(const op::Binary* binary) override;
        virtual MaybeError AddClamp(const op::Clamp* clamp) override;
        virtual MaybeError AddConv2d(const op::Conv2d* conv2d) override;
        virtual MaybeError AddPad(const op::Pad* pad) override;
        virtual MaybeError AddPool2d(const op::Pool2d* pool2d) override;
        virtual MaybeError AddReduceMean(const op::ReduceMean* reduceMean) override;
        virtual MaybeError AddReshape(const op::Reshape* reshape) override;
        virtual MaybeError AddTranspose(const op::Transpose* transpose) override;
        virtual MaybeError AddUnary(const op::Unary* unary) override;
        virtual MaybeError AddConcat(const op::Concat* concat) override;
        virtual MaybeError AddGemm(const op::Gemm* Gemm) override;
        virtual MaybeError AddInstanceNorm(const op::InstanceNorm* InstanceNorm) override;
        virtual MaybeError Finish() override;

      private:
        void CompileImpl(BuildGraphCallbackDelegate delegate) override;
        void ComputeImpl(NamedInputsBase* inputs,
                         MLComputeGraphCallback callback,
                         void* userdata,
                         NamedOutputsBase* outputs) override;

        MLBuildGraphStatus CompileSyncImpl() override;
        MLComputeGraphStatus ComputeSyncImpl(NamedInputsBase* inputs,
                                             NamedOutputsBase* outputs) override;

        MLComputeGraphStatus GenericComputeImpl(NamedInputsBase* inputs,
                                                NamedOutputsBase* outputs,
                                                MLComputeGraphCallback callback = nullptr,
                                                void* userdata = nullptr);

        ie_model_t* mIeModel;
        ie_compilation_t* mIeCompilation;

        // Map the input name to IE internal id
        std::map<std::string, std::string> mInputIdMap;
        // Map the IE internal id to output name
        std::map<std::string, std::string> mOutputNameMap;
        // Map the operand to IE internal id
        std::map<const OperandBase*, std::string> mOperandIdMap;
        // store the constant operands
        std::unordered_set<const OperandBase*> mConstantSet;
    };

}}  // namespace webnn_native::ie

#endif  // WEBNN_NATIVE_IE_MODEL_IE_H_
