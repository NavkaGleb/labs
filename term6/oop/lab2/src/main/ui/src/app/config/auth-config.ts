import { AuthConfig } from 'angular-oauth2-oidc';

export const authConfig: AuthConfig = {
  issuer: 'http://localhost:8180/auth/realms/online_shop',
  clientId: 'client_front',
  responseType: 'code',
  redirectUri: window.location.origin + "/login",
  scope: 'openid profile email offline_access roles',
  disableAtHashCheck: true,
  showDebugInformation: true,
  requireHttps: false
};
