#include <viam/sdk/app/billing.hpp>

#include <grpcpp/grpcpp.h>

#include <viam/api/app/v1/billing.grpc.pb.h>
#include <viam/api/app/v1/billing.pb.h>
#include <viam/sdk/common/client_helper.hpp>
#include <viam/sdk/common/exception.hpp>
#include <viam/sdk/config/resource.hpp>
#include <viam/sdk/rpc/dial.hpp>

namespace viam {
namespace sdk {

BillingClient::BillingClient(ViamChannel channel)
    : channel_(std::move(channel)),
      stub_(viam::app::v1::BillingService::NewStub(channel_.grpc_channel())) {}

const ViamChannel& BillingClient::channel() const {
    return channel_;
}

viam::app::v1::GetCurrentMonthUsageResponse BillingClient::GetCurrentMonthUsage(
    const std::string& org_id) {
    auto response = make_client_helper(this, *stub_, &viam::app::v1::BillingService::Stub::GetCurrentMonthUsage)
                        .with([&](viam::app::v1::GetCurrentMonthUsageRequest& req) {
                            req.set_org_id(org_id);
                        })
                        .invoke([](const viam::app::v1::GetCurrentMonthUsageResponse& resp) {
                            return resp;
                        });
    return response;
}

viam::app::v1::GetOrgBillingInformationResponse BillingClient::GetOrgBillingInformation(
    const std::string& org_id) {
    auto response = make_client_helper(this, *stub_, &viam::app::v1::BillingService::Stub::GetOrgBillingInformation)
                        .with([&](viam::app::v1::GetOrgBillingInformationRequest& req) {
                            req.set_org_id(org_id);
                        })
                        .invoke([](const viam::app::v1::GetOrgBillingInformationResponse& resp) {
                            return resp;
                        });
    return response;
}

viam::app::v1::GetInvoicesSummaryResponse BillingClient::GetInvoicesSummary(
    const std::string& org_id) {
    auto response = make_client_helper(this, *stub_, &viam::app::v1::BillingService::Stub::GetInvoicesSummary)
                        .with([&](viam::app::v1::GetInvoicesSummaryRequest& req) {
                            req.set_org_id(org_id);
                        })
                        .invoke([](const viam::app::v1::GetInvoicesSummaryResponse& resp) {
                            return resp;
                        });
    return response;
}

std::vector<uint8_t> BillingClient::GetInvoicePdf(const std::string& id,
                                                  const std::string& org_id) {
    std::vector<uint8_t> result_bytes;
    make_client_helper(this, *stub_, &viam::app::v1::BillingService::Stub::GetInvoicePdf)
        .with([&](viam::app::v1::GetInvoicePdfRequest& req) {
            req.set_id(id);
            req.set_org_id(org_id);
        })
        .invoke_stream([&](const viam::app::v1::GetInvoicePdfResponse& resp) {
            const std::string& chunk = resp.chunk();
            result_bytes.insert(result_bytes.end(), chunk.begin(), chunk.end());
            return true;
        });
    return result_bytes;
}

void BillingClient::SendPaymentRequiredEmail(const std::string& customer_org_id,
                                             const std::string& billing_owner_org_id) {
    make_client_helper(this, *stub_, &viam::app::v1::BillingService::Stub::SendPaymentRequiredEmail)
        .with([&](viam::app::v1::SendPaymentRequiredEmailRequest& req) {
            req.set_customer_org_id(customer_org_id);
            req.set_billing_owner_org_id(billing_owner_org_id);
        })
        .invoke();
}

viam::app::v1::GetAvailableBillingTiersResponse BillingClient::GetAvailableBillingTiers() {
    auto response = make_client_helper(this, *stub_, &viam::app::v1::BillingService::Stub::GetAvailableBillingTiers)
                        .invoke([](const viam::app::v1::GetAvailableBillingTiersResponse& resp) {
                            return resp;
                        });
    return response;
}

void BillingClient::UpdateOrganizationBillingTier(const std::string& organization_id,
                                                  const std::string& billing_tier) {
    make_client_helper(this, *stub_, &viam::app::v1::BillingService::Stub::UpdateOrganizationBillingTier)
        .with([&](viam::app::v1::UpdateOrganizationBillingTierRequest& req) {
            req.set_organization_id(organization_id);
            req.set_billing_tier(billing_tier);
        })
        .invoke();
}

viam::app::v1::ChargeOrganizationResponse BillingClient::ChargeOrganization(
    const std::string& org_id_to_charge,
    double subtotal,
    double tax,
    const std::string& description,
    const std::string& org_id_for_branding,
    bool disable_confirmation_email) {
    auto response = make_client_helper(this, *stub_, &viam::app::v1::BillingService::Stub::ChargeOrganization)
                        .with([&](viam::app::v1::ChargeOrganizationRequest& req) {
                            req.set_org_id_to_charge(org_id_to_charge);
                            req.set_subtotal(subtotal);
                            req.set_tax(tax);
                            if (!description.empty()) {
                                req.set_description(description);
                            }
                            if (!org_id_for_branding.empty()) {
                                req.set_org_id_for_branding(org_id_for_branding);
                            }
                            req.set_disable_confirmation_email(disable_confirmation_email);
                        })
                        .invoke([](const viam::app::v1::ChargeOrganizationResponse& resp) {
                            return resp;
                        });
    return response;
}

[[deprecated("Use ChargeOrganization instead.")]]
viam::app::v1::CreateInvoiceAndChargeImmediatelyResponse BillingClient::CreateInvoiceAndChargeImmediately(
    const std::string& org_id_to_charge,
    double amount,
    const std::string& description,
    const std::string& org_id_for_branding,
    bool disable_email) {
    viam::app::v1::ChargeOrganizationResponse charge_response = ChargeOrganization(
        org_id_to_charge,
        amount, // Assuming 'amount' from deprecated method maps to 'subtotal'
        0.0,    // Assuming 'tax' is 0 for the deprecated flat fee invoice
        description,
        org_id_for_branding,
        disable_email); // disable_email maps to disable_confirmation_email

    viam::app::v1::CreateInvoiceAndChargeImmediatelyResponse response;
    response.set_invoice_id(charge_response.invoice_id());
    return response;
}

}  // namespace sdk
}  // namespace viam